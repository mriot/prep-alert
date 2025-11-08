#include "Events.h"
#include <Common/Globals.h>
#include <Common/Utils.h>
#include <format>
#include <mumble/Mumble.h>
#include <string>

void OnMumbleIdentityUpdated(void *eventData)
{
    G::MumbleIdentity = static_cast<Mumble::Identity *>(eventData);
    if (!G::MumbleIdentity)
        return;

    if (G::MumbleIdentity->MapID == G::CurrentMapID)
        return;

    // on map change reset sector ID to avoid false positives
    G::CurrentMapID    = G::MumbleIdentity->MapID;
    G::CurrentSectorID = 0;

    if (!G::SupportedMaps.contains(G::CurrentMapID))
    {
        G::IsOnSupportedMap = false;
        Log::Debug((std::format("Map {} is NOT supported", G::CurrentMapID)).c_str());

        return;
    }

    G::IsOnSupportedMap = true;
    Log::Debug((std::format("Map {} is supported", G::CurrentMapID)).c_str());
}
