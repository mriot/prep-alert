#include "Controller.h"
#include "UI/Debug/Debug.h"
#include <Common/Globals.h>
#include <Common/Types.h>
#include <Data/SettingsManager.h>
#include <gw2re/Game/Char/ChBuffMgr.h>
#include <gw2re/Game/Char/ChCliContext.h>
#include <gw2re/Game/Char/ChCombatant.h>
#include <gw2re/Game/Char/Character.h>
#include <gw2re/Game/PropContext.h>
#include <UI/Overlay/Overlay.h>
#include <chrono>
#include <nexus/Nexus.h>
#include <vector>


///----------------------------------------------------------------------------------------------------
/// Controller Internal Functions
///----------------------------------------------------------------------------------------------------

namespace
{
    bool IsPlayerInSector(const Vec2 &pos, const Sector &sector, const int floorLevel)
    {
        if (sector.bounds.size() < 3)
            return false;

        // don't check floor level if it's 0 - our detection system prob didn't pick it up (yet)
        if (floorLevel != 0 && std::ranges::find(sector.floors, floorLevel) == sector.floors.end())
        {
            return false;
        }

        bool inside        = false;
        const size_t count = sector.bounds.size();

        for (size_t i = 0, j = count - 1; i < count; j = i++)
        {
            const auto &pi = sector.bounds[i];
            const auto &pj = sector.bounds[j];

            // ngl this is some ray-casting shit from stackoverflow
            const bool intersect =
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

        GW2RE::CCharacter character    = cctx.GetControlledCharacter();
        GW2RE::CCombatant combatant    = character.GetCombatant();
        GW2RE::CBuffMgr buffMgr        = combatant.GetBuffMgr();
        const GW2RE::BuffBar_t buffBar = buffMgr.GetBuffBar();

        for (size_t i = 0; i < buffBar.Capacity; i++)
        {
            const auto &entry = buffBar.Entries[i];

            if (entry.Hash == 0 || !entry.KVP.Value)
                continue;

            const int effectID = static_cast<int>(entry.KVP.Value->EffectID);

            if (effectID == buff.id)
            {
                // Log::Debug("Player has desired buff: " + BuffDefs.at(buff.id).name);
                return true;
            }
        }

        return false; // buff not found
    }

    // determines which buff to show based on sector/default buffs and combat state
    std::optional<Buff> GetBuffToShow(
        const std::optional<Buff> &sectorBuff,
        const std::optional<Buff> &defaultBuff,
        const bool isInCombat,
        bool &genericShownFlag)
    {
        if (!sectorBuff.has_value() && !defaultBuff.has_value())
            return std::nullopt;

        // helper to check if buff id > 0 (i.e. a "real" buff)
        auto isRealBuff = [](const auto &buff) {
            return buff.has_value() && buff->id > 0;
        };

        // reset generic flag when real buffs become available
        if (isRealBuff(sectorBuff) || isRealBuff(defaultBuff))
        {
            genericShownFlag = false;
        }
        // no real buffs and default buffs disabled
        else if (!SettingsManager::GetShownBuffTypes().defaultBuffs)
        {
            return std::nullopt;
        }

        // determine which buff to show (sector takes priority)
        const auto &buffToShow = sectorBuff.has_value() ? sectorBuff : defaultBuff;

        // generic (custom) buff handling
        if (buffToShow->id < 0)
        {
            if (isInCombat) // hide generic buff upon entering combat
                genericShownFlag = true;
            else if (!genericShownFlag) // show only once until combat
                return buffToShow;

            return std::nullopt; // either in combat or already shown
        }

        // show real buff
        return buffToShow;
    }
}

void OnRender()
{
    static std::vector<Buff> buffReminders;
    static auto lastFrameTime       = std::chrono::steady_clock::now();
    static bool genericSigilShown   = false;
    static bool genericUtilityShown = false;

    if (SettingsManager::IsDebugWindowEnabled())
        DebugOverlay::OnDebugRender(buffReminders);

    if (!G::NexusLink->IsGameplay)
        return;

    // render settings overlay regardless of map support status to be able to drag it on any map
    if (UIState::IsOptionsPaneOpen)
    {
        if (SettingsManager::GetShownBuffTypes().utility)
        {
            buffReminders.push_back(Buff(-1, "\"Potion of Calibration\""));
        }
        if (SettingsManager::GetShownBuffTypes().sigil)
        {
            buffReminders.push_back(Buff(-2, "\"Sigil of the Tinkerer\""));
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
    auto const elapsed          = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime).count();

    if (elapsed < 500)
        return;

    lastFrameTime = currentFrameTime;

    const float map_x    = G::MumbleLink->Context.Compass.PlayerPosition.X;
    const float map_y    = G::MumbleLink->Context.Compass.PlayerPosition.Y;
    const float player_y = G::MumbleLink->AvatarPosition.Y; // vertical position

    auto &currentMap = G::MapDataMap.at(G::CurrentMapID);

    // special case for Lonely Tower Fractal - despite actually having multiple floors, the game treats it as a single floor
    if (currentMap.id == 1538 && player_y > 890.0f) // at this height we're definitely on the upper floor
    {
        G::CurrentMapFloor = 64; // default is 63
    }

    for (auto &sector : currentMap.sectors)
    {
        if (!IsPlayerInSector({map_x, map_y}, sector, G::CurrentMapFloor))
        {
            continue;
        }

        if (sector.id != G::CurrentSectorID)
        {
            G::CurrentSectorID = sector.id;
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
                addBuffReminder(buff);
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
                addBuffReminder(buff);
        }

        break; // player can be in only one sector
    }
}
