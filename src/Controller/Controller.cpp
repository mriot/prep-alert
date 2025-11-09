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

            if (static_cast<int>(buffBar.Entries[i].KVP.Value->EffectID) == buff.id)
            {
                // Log::Debug("Player has desired buff: " + BuffDefs.at(buff.id).name);
                return true;
            }
        }

        return false;
    }
} // namespace

void OnRender()
{
    static std::vector<Buff> buffs;
    static auto last           = std::chrono::steady_clock::now();
    static bool wasOptionsOpen = false;

    if (!G::NexusLink->IsGameplay)
        return;

    // needed to disable drag mode when closing settings
    if (G::IsOptionsPaneOpen)
    {
        wasOptionsOpen       = true;
        G::IsOptionsPaneOpen = false;
    }
    else if (wasOptionsOpen)
    {
        SettingsManager::SetOverlayDragEnabled(false);
        wasOptionsOpen = false;
    }

    // run regardless of map support status to be able to drag overlay on any map
    if (SettingsManager::IsOverlayDragEnabled())
    {
        Overlay::RenderOverlay({Buff(-1, "I'm just a placeholder"), Buff(-1, "While you change settings")});
        return; // no need to go further
    }

    if (!G::IsOnSupportedMap)
    {
        buffs.clear(); // avoids ugly leftovers when changing map
        return;
    }

    // let the overlay handle the case of no buffs to show (needed for some cleanup)
    Overlay::RenderOverlay(buffs);

    // avoid checking map and sector stuff each frame
    const auto now = std::chrono::steady_clock::now();
    auto elapsed   = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();

    if (elapsed < 500)
        return;

    last = now;

    // global player position
    float x = G::MumbleLink->Context.Compass.PlayerPosition.X;
    float y = G::MumbleLink->Context.Compass.PlayerPosition.Y;

    auto &currentMap = G::MapDataMap.at(G::CurrentMapID);

    for (auto &sector : currentMap.sectors)
    {
        if (!IsPlayerInSector({x, y}, sector.bounds))
        {
            continue; // out of bounds eh?
        }

        if (sector.id != G::CurrentSectorID)
        {
            G::CurrentSectorID = sector.id;
            Log::Info(std::format("Sector: {} ({})", sector.name, sector.id));
        }

        // we have to check buffs even if sector didn't change
        // in case a buff ran out or equipment changed etc.

        buffs.clear();

        auto addBuffReminder = [&](const std::optional<Buff> &buffOpt) {
            if (!buffOpt.has_value())
                return false; // not specified -> try fallback if any

            // either we add a reminder or player has buff already
            if (!PlayerHasBuff(buffOpt.value()))
            {
                buffs.push_back(buffOpt.value());
            }
            return true;
        };

        // utility
        if (!addBuffReminder(sector.buffs.utility))
        {
            addBuffReminder(currentMap.default_buffs.utility);
        }

        // sigil
        if (!addBuffReminder(sector.buffs.sigil))
        {
            addBuffReminder(currentMap.default_buffs.sigil);
        }

        break; // player can be in only one sector
    }
}
