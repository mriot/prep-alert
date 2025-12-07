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
        bool isGenericDismissed         = false;
        bool hasGenericRemindersEnabled = true;
        bool isPlayerInCombat           = false;

        bool isGenericBuff() const { return buff.has_value() && buff->id < 0; }

        void clearBuff() { buff = {}; }

        void reset()
        {
            buff.reset();
            isGenericDismissed = false;
        }
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
            const auto &entry = buffBar.Entries[i];
            if (entry.Hash == 0 || !entry.KVP.Value)
                continue;

            ids.insert(static_cast<int>(entry.KVP.Value->EffectID));
        }
        return ids;
    }

    bool shouldAddBuffReminder(const BuffReminder &reminder, const std::unordered_set<int> &activeBuffIds)
    {
        if (!reminder.buff.has_value())
            return false;

        if (reminder.isGenericBuff())
        {
            if (!reminder.hasGenericRemindersEnabled)
                return false;

            // check if any specific buff of the same type is active
            for (const auto &[buffId, buffDef] : BuffDefs)
            {
                if (buffDef.type != reminder.buff->type)
                    continue;

                if (activeBuffIds.contains(buffId))
                    return true; // some specific buff active, show reminder

            }
            return false;
        }

        // player has buff - now check whether it SHOULD be active
        if (activeBuffIds.contains(reminder.buff->id))
            return reminder.buff->type == BuffType::Reset; // show reminder only if the player should remove it

        // buff not active - show reminder
        return reminder.buff->type != BuffType::Reset; // don't show reminder to remove a buff that's not active
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
        DebugOverlay::RenderDebugOverlay(buffReminders);

    if (!G::NexusLink->IsGameplay || G::MumbleLink->Context.IsMapOpen)
        return;

    const MapTypeReminder reminderCfg = SettingsManager::GetReminder(G::CurrentContinent);

    // set up the draggable overlay when options are open
    if (UIState::IsOptionsPaneOpen)
    {
        buffReminders.clear();

        const Buff genericUtilityBuff(BuffIds::GENERIC_ENHANCEMENT, "\"Potion of Calibration\"");
        const Buff genericSigilBuff(BuffIds::NIGHT_SIGIL, "\"Sigil of the Night\"");
        const Buff genericSigilSlayingBuff(BuffIds::GENERIC_SIGIL, "\"Sigil of Slaying\"");

        if (G::CurrentContinent == Continent::Unknown)
        {
            buffReminders.push_back(genericUtilityBuff);
            buffReminders.push_back(genericSigilBuff);
            buffReminders.push_back(genericSigilSlayingBuff);
        }
        else
        {
            if (!reminderCfg.enabled)
                return;
            if (reminderCfg.utility)
                buffReminders.push_back(genericUtilityBuff);
            if (reminderCfg.sigil)
                buffReminders.push_back(genericSigilBuff);
            if (reminderCfg.sigilSlaying)
                buffReminders.push_back(genericSigilSlayingBuff);
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
    if (!G::IsOnSupportedMap || !reminderCfg.enabled)
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

    const auto activeBuffIds  = buildActiveBuffIdSet();
    const bool playerInCombat = static_cast<bool>(G::MumbleLink->Context.IsInCombat);
    const auto &currentMap    = G::CurrentMapData;
    const float mapX          = G::MumbleLink->Context.Compass.PlayerPosition.X;
    const float mapY          = G::MumbleLink->Context.Compass.PlayerPosition.Y;
    const float playerY       = G::MumbleLink->AvatarPosition.Y; // there is no vertical position in compass

    // TODO refactor this
    utilityReminder.clearBuff();
    utilityReminder.hasGenericRemindersEnabled = reminderCfg.defaultBuffs;
    utilityReminder.isPlayerInCombat           = playerInCombat;

    sigilReminder.clearBuff();
    sigilReminder.hasGenericRemindersEnabled = reminderCfg.defaultBuffs;
    sigilReminder.isPlayerInCombat           = playerInCombat;

    sigilSlayingReminder.clearBuff();
    sigilSlayingReminder.hasGenericRemindersEnabled = reminderCfg.defaultBuffs;
    sigilSlayingReminder.isPlayerInCombat           = playerInCombat;

    // certain maps need special floor level overrides based on player Y position
    if (const auto override = getFloorLevelOverride(currentMap.id, playerY))
        G::CurrentMapFloor  = *override;

    G::CurrentSectorID = 0;

    // check for sector specific buffs
    for (auto &sector : currentMap.sectors)
    {
        if (!IsPlayerInSector({mapX, mapY}, sector, G::CurrentMapFloor))
            continue;

        G::CurrentSectorID = sector.id;

        if (reminderCfg.utility)
            utilityReminder.buff = sector.buffs.utility;

        if (reminderCfg.sigil)
            sigilReminder.buff = sector.buffs.sigil;

        if (reminderCfg.sigilSlaying)
            sigilSlayingReminder.buff = sector.buffs.sigilSlaying;

        break; // player can be in only one sector at a time
    }

    // handle map's default buffs if no sector buffs found

    if (!utilityReminder.buff.has_value() && reminderCfg.utility)
        utilityReminder.buff = currentMap.default_buffs.utility;

    if (!sigilReminder.buff.has_value() && reminderCfg.sigil)
        sigilReminder.buff = currentMap.default_buffs.sigil;

    if (!sigilSlayingReminder.buff.has_value() && reminderCfg.sigilSlaying)
        sigilSlayingReminder.buff = currentMap.default_buffs.sigilSlaying;

    // build final buff reminder list

    if (shouldAddBuffReminder(utilityReminder, activeBuffIds))
        buffReminders.push_back(utilityReminder.buff.value());

    if (shouldAddBuffReminder(sigilReminder, activeBuffIds))
        buffReminders.push_back(sigilReminder.buff.value());

    if (shouldAddBuffReminder(sigilSlayingReminder, activeBuffIds))
        buffReminders.push_back(sigilSlayingReminder.buff.value());
}
