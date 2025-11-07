#include "Logic.h"
#include <Common/Globals.h>
#include <Common/Types.h>
#include <Common/Utils.h>
#include <Data/SettingsManager.h>
#include <UI/Overlay/Overlay.h>
#include <buffs.h>
#include <chrono>
#include <format>
#include <nexus/Nexus.h>
#include <string>
#include <vector>

bool IsPlayerInSector(const Vec2 &pos, const std::vector<Vec2> &bounds)
{
    if (bounds.size() < 3)
        return false;

    bool inside  = false;
    size_t count = bounds.size();

    for (size_t i = 0, j = count - 1; i < count; j = i++)
    {
        const auto &pi = bounds[i];
        const auto &pj = bounds[j];

        bool intersect =
            ((pi.y > pos.y) != (pj.y > pos.y)) &&
            (pos.x < (pj.x - pi.x) * (pos.y - pi.y) / (pj.y - pi.y) + pi.x);

        if (intersect)
            inside = !inside;
    }

    return inside;
}

void OnRender()
{
    BuffTest();

    if (SettingsManager::IsOverlayDragEnabled())
    {
        Overlay::RenderOverlay(Buff(9963, "DRAG ME BABY!", ""));
        return; // no need to do the rest here
    }

    static Buff buff;
    static auto last = std::chrono::steady_clock::now();

    auto now     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();

    if (!G::NexusLink->IsGameplay)
        return;

    if (G::CurrentMapID == -1 || !G::SupportedMaps.contains(G::CurrentMapID))
        return;

    Overlay::RenderOverlay(buff); // must be called each frame

    if (elapsed < 1000)
        return;

    last = now;

    if (!G::MapDataMap.contains(G::CurrentMapID))
        return;

    // global player position
    double x = G::MumbleLink->Context.Compass.PlayerPosition.X;
    double y = G::MumbleLink->Context.Compass.PlayerPosition.Y;

    auto &currentMap = G::MapDataMap.at(G::CurrentMapID);

    for (auto &sector : currentMap.sectors)
    {
        if (!IsPlayerInSector({x, y}, sector.bounds) || sector.id == G::CurrentSectorID)
        {
            continue;
        }

        G::CurrentSectorID = sector.id;

        Log::Info(std::format("Sector: {} ({})", sector.name, sector.id).c_str());

        if (sector.buffs.utility.has_value())
        {
            buff = sector.buffs.utility.value();
            Overlay::ShowOverlay(SettingsManager::GetOverlayTimeoutSeconds());
        }
        else if (currentMap.default_buffs.utility.has_value())
        {
            buff = currentMap.default_buffs.utility.value();
            Overlay::ShowOverlay(SettingsManager::GetOverlayTimeoutSeconds());
        }
        else
        {
            Log::Warning("No utility buff found for this sector or map default.");
        }

        break;
    }
}
