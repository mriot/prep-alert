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

    // on map change reset sector ID to avoid false positives
    G::CurrentMapID    = G::MumbleIdentity->MapID;
    G::CurrentSectorID = 0;

    if (!G::SupportedMaps.contains(G::CurrentMapID))
    {
        G::IsOnSupportedMap = false;
        return;
    }

    G::IsOnSupportedMap = true;
}
