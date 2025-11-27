#include "Events.h"
#include <Common/Globals.h>
#include <mumble/Mumble.h>

namespace Events
{
    void OnMumbleIdentityUpdated(void *eventData)
    {
        G::MumbleIdentity = static_cast<Mumble::Identity *>(eventData);
        if (!G::MumbleIdentity)
            return;

        const uint32_t updatedMapID = G::MumbleIdentity->MapID;
        if (updatedMapID == G::CurrentMapData.id)
            return;

        const auto mapEntry = G::MapDataMap.find(updatedMapID);
        const bool mapFound = mapEntry != G::MapDataMap.end();

        if (mapFound)
        {
            const auto &mapData = mapEntry->second;
            G::CurrentContinent = mapData.continent_id == 1 ? Continent::Tyria : Continent::Mists;
            G::CurrentMapData   = mapData;
        }
        else
        {
            G::CurrentContinent = Continent::Unknown;
            G::CurrentMapData   = {};
        }

        G::IsOnSupportedMap = mapFound;
        G::CurrentSectorID  = 0; // actual sector detected elsewhere
    }
}
