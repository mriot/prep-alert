#include "Events.h"
#include "Data/SettingsManager.h"
#include <Common/Globals.h>
#include <mumble/Mumble.h>


bool isMapSupported(const MapData &mapData)
{
    const MapTypes mapTypes = SettingsManager::GetMapTypes();

    if (!mapTypes.dungeons && mapData.continent_id == 1)
        return false;
    if (!mapTypes.fractals && mapData.continent_id == 2)
        return false;

    return G::SupportedMaps.contains(G::CurrentMapID);
}

void OnMumbleIdentityUpdated(void *eventData)
{
    G::MumbleIdentity = static_cast<Mumble::Identity *>(eventData);

    if (!G::MumbleIdentity)
        return;

    if (G::MumbleIdentity->MapID == G::CurrentMapID)
        return;

    G::CurrentMapID    = G::MumbleIdentity->MapID;
    G::CurrentSectorID = 0;

    const auto mapIt = G::MapDataMap.find(G::CurrentMapID);
    if (mapIt == G::MapDataMap.end())
    {
        G::IsOnSupportedMap = false;
        return;
    }

    G::IsOnSupportedMap = isMapSupported(mapIt->second);
}
