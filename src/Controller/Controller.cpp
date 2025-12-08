#include "Controller.h"

#include "Common/BuffData.h"
#include "Common/Constants.h"
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
        void reset() { buff.reset(); }
        bool shouldRemoveBuff() const { return buff.has_value() && buff->id < 0; }
    };

    // artificially adjust the floor level based on map ID and player Y position if needed
    std::optional<int> getFloorLevelOverride(const int mapId, const float playerY)
    {
        switch (mapId)
        {
        case MapIds::LONELY_TOWER_FRACTAL:
            if (playerY > 890.0f)
                return 64; // the lower default floor is 63
            break;
        case MapIds::CRUCIBLE_OF_ETERNITY_STORY:
            if (playerY < 140.0f)
                return -12; // the default floor is -11
            break;
        case MapIds::CRUCIBLE_OF_ETERNITY:
            if (playerY > 200.0f)
                return -10; // the default floor is -11
            break;
        default:
            break;
        }

        return std::nullopt;
    }

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
            const auto &[KVP, Hash] = buffBar.Entries[i];
            if (Hash == 0 || !KVP.Value)
                continue;

            ids.insert(static_cast<int>(KVP.Value->EffectID));
        }
        return ids;
    }

    bool shouldAddBuffReminder(BuffReminder &reminder, const std::unordered_set<int> &activeBuffIds)
    {
        if (!reminder.buff.has_value())
            return false;

        if (reminder.shouldRemoveBuff())
        {
            // check if any specific buff of the same type is active
            for (const auto &[buffId, buffDef] : BuffData::BuffDefs)
            {
                if (buffDef.type != reminder.buff->type)
                    continue;

                // TODO handle elsewhere and keep function pure
                if (activeBuffIds.contains(buffId))
                {
                    // load the specific buff info into the reminder
                    reminder.buff->id     = buffId;
                    reminder.buff->remove = true;
                    return true; // some specific buff active, show reminder to replace it
                }

            }
            return false; // no specific buff of this type active, no reminder needed
        }

        return !activeBuffIds.contains(reminder.buff->id);
    }

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
    static BuffReminder sigilSlayingReminder;

    if (SettingsManager::IsDebugWindowEnabled())
        Debug::RenderDebugOverlay(buffReminders);

    if (!G::NexusLink->IsGameplay || G::MumbleLink->Context.IsMapOpen)
        return;

    const MapTypeReminder reminderCfg = SettingsManager::GetReminder(WorldState::CurrentContinent);

    if (UIState::IsOptionsPaneOpen)
    {
        buffReminders.clear();

        constexpr Buff optionsUtilityBuff(BuffIds::REMOVE_ENHANCEMENT);
        constexpr Buff optionsNightSigil(BuffIds::NIGHT_SIGIL);
        constexpr Buff optionsSlayingSigil(BuffIds::REMOVE_SIGIL);

        // for unsupported maps, always show all buffs in options pane
        if (WorldState::CurrentContinent == Continent::Unknown) // TODO: works, but maybe change to a better check later
        {
            buffReminders.push_back(optionsUtilityBuff);
            buffReminders.push_back(optionsNightSigil);
            buffReminders.push_back(optionsSlayingSigil);
        }
        else
        {
            if (!reminderCfg.enabled)
                return;
            if (reminderCfg.utility)
                buffReminders.push_back(optionsUtilityBuff);
            if (reminderCfg.nightSigil)
                buffReminders.push_back(optionsNightSigil);
            if (reminderCfg.slayingSigil)
                buffReminders.push_back(optionsSlayingSigil);
        }
    }

    // to prevent flicker, the previous frame’s data is rendered (map/sector checks are throttled below)
    Overlay::RenderOverlay(buffReminders);

    // skip further processing if options pane is open
    if (UIState::IsOptionsPaneOpen)
    {
        UIState::IsOptionsPaneOpen = false; // this is set to true by options render each frame it's open
        return;
    }

    // reset reminders if not on supported map or reminders disabled
    if (!WorldState::IsOnSupportedMap || !reminderCfg.enabled)
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

    const auto activeBuffIds = buildActiveBuffIdSet();
    const auto &currentMap   = WorldState::CurrentMapData;
    const float mapX         = G::MumbleLink->Context.Compass.PlayerPosition.X;
    const float mapY         = G::MumbleLink->Context.Compass.PlayerPosition.Y;
    const float playerY      = G::MumbleLink->AvatarPosition.Y; // there is no vertical position in compass
    int actualFloorLevel     = WorldState::CurrentMapFloor;

    // certain maps need special floor level overrides based on player Y position
    if (const auto override = getFloorLevelOverride(currentMap.id, playerY))
        actualFloorLevel    = *override;

    Debug::Info.actualFloorLevel = actualFloorLevel;

    utilityReminder.reset();
    sigilReminder.reset();
    sigilSlayingReminder.reset();

    WorldState::CurrentSectorID = 0;

    // check for sector specific buffs
    for (const auto &sector : currentMap.sectors)
    {
        if (!IsPlayerInSector({mapX, mapY}, sector, actualFloorLevel))
            continue;

        WorldState::CurrentSectorID = sector.id;

        if (reminderCfg.utility)
            utilityReminder.buff = sector.buffs.utility;

        if (reminderCfg.nightSigil)
            sigilReminder.buff = sector.buffs.sigilNight;

        if (reminderCfg.slayingSigil)
            sigilSlayingReminder.buff = sector.buffs.sigilSlaying;

        break; // player can be in only one sector at a time
    }

    // handle map's default buffs if no sector buffs found

    if (!utilityReminder.buff.has_value() && reminderCfg.utility)
        utilityReminder.buff = currentMap.default_buffs.utility;

    if (!sigilReminder.buff.has_value() && reminderCfg.nightSigil)
        sigilReminder.buff = currentMap.default_buffs.sigilNight;

    if (!sigilSlayingReminder.buff.has_value() && reminderCfg.slayingSigil)
        sigilSlayingReminder.buff = currentMap.default_buffs.sigilSlaying;

    // build final buff reminder list

    if (shouldAddBuffReminder(utilityReminder, activeBuffIds))
        buffReminders.push_back(utilityReminder.buff.value());

    if (shouldAddBuffReminder(sigilReminder, activeBuffIds))
        buffReminders.push_back(sigilReminder.buff.value());

    if (shouldAddBuffReminder(sigilSlayingReminder, activeBuffIds))
        buffReminders.push_back(sigilSlayingReminder.buff.value());
}
