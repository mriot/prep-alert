#include "Events.h"
#include <Common/Globals.h>
#include <Common/Utils.h>
#include <Logic/Logic.h>
#include <format>
#include <mumble/Mumble.h>
#include <nexus/Nexus.h>
#include <string>

void OnMumbleIdentityUpdated(void *eventData)
{
    G::MumbleIdentity = static_cast<Mumble::Identity *>(eventData);
    if (!G::MumbleIdentity)
        return;

    if (G::MumbleIdentity->MapID == G::CurrentMapID)
        return;

    G::CurrentMapID    = G::MumbleIdentity->MapID;
    G::CurrentSectorID = -1;

    if (!G::SupportedMaps.contains(G::CurrentMapID))
    {
        Log::Info((std::format("Map {} is NOT supported", G::CurrentMapID)).c_str());
        G::APIDefs->GUI_Deregister(OnRender);

        return;
    }

    Log::Info((std::format("Map {} is supported", G::CurrentMapID)).c_str());
    G::APIDefs->GUI_Register(ERenderType::RT_Render, OnRender);
}
