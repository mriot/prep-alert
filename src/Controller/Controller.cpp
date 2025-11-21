#include "Controller.h"

#include "Common/Utils.h"
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
#include <unordered_set>


namespace
{
    struct BuffReminder
    {
        std::optional<Buff> buff;
        bool isGenericDismissed = false;
        bool showGenericBuff    = true;
        bool playerInCombat     = false;

        bool isGenericBuff() { return buff.has_value() && buff->id < 0; }

        void clearBuff() { buff = {}; }

        void reset()
        {
            buff.reset();
            isGenericDismissed = false;
        }
    };

    std::unordered_set<int> buildActiveBuffIdSet()
    {
        std::unordered_set<int> ids;

        GW2RE::CPropContext propCtx    = GW2RE::CPropContext::Get();
        GW2RE::CCharCliContext cctx    = propCtx.GetCharCliCtx();
        GW2RE::CCharacter character    = cctx.GetControlledCharacter();
        GW2RE::CCombatant combatant    = character.GetCombatant();
        GW2RE::CBuffMgr buffMgr        = combatant.GetBuffMgr();
        const GW2RE::BuffBar_t buffBar = buffMgr.GetBuffBar();

        for (size_t i = 0; i < buffBar.Capacity; ++i)
        {
            const auto &entry = buffBar.Entries[i];
            if (entry.Hash == 0 || !entry.KVP.Value)
                continue;

            ids.insert(static_cast<int>(entry.KVP.Value->EffectID));
        }
        return ids;
    }

    bool shouldAddBuffReminder(BuffReminder &reminder, const std::unordered_set<int> &activeBuffIds)
    {
        if (!reminder.buff.has_value())
            return false;

        if (reminder.isGenericBuff())
        {
            // player disabled generic buff reminders
            if (!reminder.showGenericBuff)
                return false;

            // hide generic buff upon entering combat and then keep it hidden
            if (reminder.playerInCombat)
            {
                reminder.isGenericDismissed = true;
                return false;
            }
            return !reminder.isGenericDismissed;
        }

        if (activeBuffIds.contains(reminder.buff->id))
            return false;

        return true;
    };

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
}

void OnRender()
{
    static auto lastFrameTime = std::chrono::steady_clock::now();
    static std::vector<Buff> buffReminders;
    static BuffReminder utilityReminder;
    static BuffReminder sigilReminder;

    if (SettingsManager::IsDebugWindowEnabled())
        DebugOverlay::RenderDebugOverlay(buffReminders);

    if (!G::NexusLink->IsGameplay)
        return;

    // render settings overlay regardless of map support status to be able to drag it on any map
    if (UIState::IsOptionsPaneOpen)
    {
        buffReminders.clear();

        if (SettingsManager::GetShownBuffTypes().utility)
            buffReminders.push_back(Buff(-1, "\"Potion of Calibration\""));

        if (SettingsManager::GetShownBuffTypes().sigil)
            buffReminders.push_back(Buff(-2, "\"Sigil of the Tinkerer\""));
    }

    // to prevent flicker, the previous frame’s data is rendered since map/sector checks are throttled below
    if (!buffReminders.empty())
        Overlay::RenderOverlay(buffReminders);

    // skip further processing if options pane is open
    if (UIState::IsOptionsPaneOpen)
    {
        UIState::IsOptionsPaneOpen = false; // this is set to true by options render each frame it's open
        return;
    }

    // reset everything if not on a supported map
    // TODO we should reset on every map change actually
    if (!G::IsOnSupportedMap)
    {
        sigilReminder.reset();
        utilityReminder.reset();
        buffReminders.clear();
        return;
    }

    // throttle map/sector checks
    const auto currentFrameTime = std::chrono::steady_clock::now();
    auto const elapsedTime      = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime).count();

    if (elapsedTime < 500)
        return;

    lastFrameTime = currentFrameTime;

    buffReminders.clear(); // clear for new data

    const auto mapIt = G::MapDataMap.find(G::CurrentMapID);
    if (mapIt == G::MapDataMap.end()) // sanity check for map data - should never happen
        return;

    const bool showGenericBuffs = SettingsManager::GetShownBuffTypes().defaultBuffs;
    const bool playerInCombat   = (bool)G::MumbleLink->Context.IsInCombat;
    const auto activeBuffIds    = buildActiveBuffIdSet();
    const auto &currentMap      = mapIt->second;
    const float mapX            = G::MumbleLink->Context.Compass.PlayerPosition.X;
    const float mapY            = G::MumbleLink->Context.Compass.PlayerPosition.Y;
    const float playerY         = G::MumbleLink->AvatarPosition.Y; // vertical position (there is no vertical position in compass)

    utilityReminder.clearBuff();
    utilityReminder.showGenericBuff = showGenericBuffs;
    utilityReminder.playerInCombat  = playerInCombat;

    sigilReminder.clearBuff();
    sigilReminder.showGenericBuff = showGenericBuffs;
    sigilReminder.playerInCombat  = playerInCombat;

    // special case for Lonely Tower Fractal - despite actually having multiple floors, the game treats it as a single floor
    if (currentMap.id == 1538 && playerY > 890.0f)
    {
        // artificially adjust the floor level when the player is on the upper floor (end boss)
        G::CurrentMapFloor = 64; // the lower default floor is 63
    }

    G::CurrentSectorID = 0;

    // check for sector specific buffs
    for (auto &sector : currentMap.sectors)
    {
        if (!IsPlayerInSector({mapX, mapY}, sector, G::CurrentMapFloor))
            continue;

        G::CurrentSectorID = sector.id;

        if (SettingsManager::GetShownBuffTypes().utility)
            utilityReminder.buff = sector.buffs.utility;

        if (SettingsManager::GetShownBuffTypes().sigil)
            sigilReminder.buff = sector.buffs.sigil;

        break; // player can be in only one sector at a time
    }

    // handle map's default buffs if no sector buffs found

    if (!utilityReminder.buff.has_value() && SettingsManager::GetShownBuffTypes().utility)
        utilityReminder.buff = currentMap.default_buffs.utility;

    if (!sigilReminder.buff.has_value() && SettingsManager::GetShownBuffTypes().sigil)
        sigilReminder.buff = currentMap.default_buffs.sigil;

    // build final buff reminder list

    if (shouldAddBuffReminder(utilityReminder, activeBuffIds))
        buffReminders.push_back(utilityReminder.buff.value());

    if (shouldAddBuffReminder(sigilReminder, activeBuffIds))
        buffReminders.push_back(sigilReminder.buff.value());
}
