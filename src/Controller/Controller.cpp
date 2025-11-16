#include "Controller.h"
#include <Common/BuffData.h>
#include <Common/Globals.h>
#include <Common/Types.h>
#include <Common/Utils.h>
#include <Data/SettingsManager.h>
#include <gw2re/Game/Char/ChBuffMgr.h>
#include <gw2re/Game/Char/ChCliContext.h>
#include <gw2re/Game/Char/ChCombatant.h>
#include <gw2re/Game/Char/Character.h>
#include <gw2re/Game/PropContext.h>
#include <UI/Overlay/Overlay.h>
#include <chrono>
#include <format>
#include <nexus/Nexus.h>
#include <string>
#include <vector>

namespace
{
    bool IsPlayerInSector(const Vec2 &pos, const std::vector<Vec2> &bounds)
    {
        if (bounds.size() < 3)
            return false;

        bool inside        = false;
        const size_t count = bounds.size();

        for (size_t i = 0, j = count - 1; i < count; j = i++)
        {
            const auto &pi = bounds[i];
            const auto &pj = bounds[j];

            // ngl this is some ray-casting shit from stackoverflow
            bool intersect =
                ((pi.y > pos.y) != (pj.y > pos.y)) &&
                (pos.x < (pj.x - pi.x) * (pos.y - pi.y) / (pj.y - pi.y) + pi.x);

            if (intersect)
                inside = !inside;
        }

        return inside;
    }

    bool PlayerHasBuff(const Buff &buff)
    {
        static GW2RE::CPropContext propCtx = GW2RE::CPropContext::Get();

        GW2RE::CCharCliContext cctx = propCtx.GetCharCliCtx();

        GW2RE::CCharacter character = cctx.GetControlledCharacter();
        GW2RE::CCombatant combatant = character.GetCombatant();
        GW2RE::CBuffMgr buffMgr     = combatant.GetBuffMgr();
        GW2RE::BuffBar_t buffBar    = buffMgr.GetBuffBar();

        for (size_t i = 0; i < buffBar.Capacity; i++)
        {
            if (buffBar.Entries[i].Hash == 0)
                continue;

            const int effectID = static_cast<int>(buffBar.Entries[i].KVP.Value->EffectID);

            if (effectID == buff.id)
            {
                // Log::Debug("Player has desired buff: " + BuffDefs.at(buff.id).name);
                return true;
            }
        }

        return false;
    }

    std::optional<Buff> GetBuffToShow(
        const std::optional<Buff> &sectorBuff,
        const std::optional<Buff> &defaultBuff,
        bool isInCombat,
        bool &genericShownFlag)
    {
        if (!sectorBuff.has_value() && !defaultBuff.has_value())
        {
            return std::nullopt;
        }

        // helper to check if buff id > 0
        auto isRealBuff = [](const auto &buff) {
            return buff.has_value() && buff->id > 0;
        };

        // reset generic flag when real buffs become available
        const bool hasRealBuff = isRealBuff(sectorBuff) || isRealBuff(defaultBuff);
        if (hasRealBuff)
        {
            genericShownFlag = false;
        }

        // determine which buff to show (sector takes priority)
        const auto &buffToShow = sectorBuff.has_value() ? sectorBuff : defaultBuff;
        const bool isGeneric   = (buffToShow->id < 0);

        if (isGeneric)
        {
            if (isInCombat)
            {
                genericShownFlag = true; // hide generic buff upon entering combat
            }
            else if (!genericShownFlag)
            {
                return (buffToShow);
            }
            return std::nullopt; // either in combat or already shown
        }

        // show real buff
        return (buffToShow);
    }
} // namespace

void OnRender()
{
    static std::vector<Buff> buffReminders;
    static auto lastFrameTime       = std::chrono::steady_clock::now();
    static bool genericSigilShown   = false;
    static bool genericUtilityShown = false;

    if (!G::NexusLink->IsGameplay)
        return;

    // run settings overlay regardless of map support status to be able to drag overlay on any map
    if (UIState::IsOptionsPaneOpen)
    {
        if (SettingsManager::GetShownBuffTypes().utility)
        {
            buffReminders.push_back(Buff(-1, "Enhancement Placeholder"));
        }
        if (SettingsManager::GetShownBuffTypes().sigil)
        {
            buffReminders.push_back(Buff(-2, "Sigil Placeholder"));
        }

        Overlay::RenderOverlay(buffReminders);
        buffReminders.clear();

        UIState::IsOptionsPaneOpen = false; // set to true by options render each frame

        return; // no need to go further
    }

    if (!G::IsOnSupportedMap)
    {
        buffReminders.clear();
        return;
    }

    Overlay::RenderOverlay(buffReminders);

    UIState::IsOptionsPaneOpen = false; // set to true by options render each frame

    // avoid checking map and sector stuff each frame
    const auto currentFrameTime = std::chrono::steady_clock::now();
    auto elapsed                = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime).count();

    if (elapsed < 500)
        return;

    lastFrameTime = currentFrameTime;

    // global player position
    const float x = G::MumbleLink->Context.Compass.PlayerPosition.X;
    const float y = G::MumbleLink->Context.Compass.PlayerPosition.Y;

    auto &currentMap = G::MapDataMap.at(G::CurrentMapID);

    for (auto &sector : currentMap.sectors)
    {
        if (!IsPlayerInSector({x, y}, sector.bounds))
        {
            continue;
        }

        if (sector.id != G::CurrentSectorID)
        {
            G::CurrentSectorID = sector.id;
            Log::Info(std::format("Sector: {} ({})", sector.name, sector.id));
        }

        // we have to check buffs even if sector didn't change
        // in case a buff ran out or equipment changed etc.

        buffReminders.clear();

        auto addBuffReminder = [&](const std::optional<Buff> &buffOpt) {
            if (!buffOpt.has_value())
                return false; // not specified

            // either we add a reminder or player has buff already
            if (!PlayerHasBuff(buffOpt.value()))
            {
                buffReminders.push_back(buffOpt.value());
            }
            return true;
        };

        if (SettingsManager::GetShownBuffTypes().utility)
        {
            const auto buff = GetBuffToShow(
                sector.buffs.utility,
                currentMap.default_buffs.utility,
                G::MumbleLink->Context.IsInCombat,
                genericUtilityShown
                );

            if (buff.has_value())
            {
                addBuffReminder(buff);
            }
        }

        if (SettingsManager::GetShownBuffTypes().sigil)
        {
            const auto buff = GetBuffToShow(
                sector.buffs.sigil,
                currentMap.default_buffs.sigil,
                G::MumbleLink->Context.IsInCombat,
                genericSigilShown
                );

            if (buff.has_value())
            {
                addBuffReminder(buff);
            }
        }

        break; // player can be in only one sector
    }
}
