#include "Events.h"
#include <Common/Globals.h>
#include <mumble/Mumble.h>


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

    G::IsOnSupportedMap = G::SupportedMaps.contains(G::CurrentMapID);
}
