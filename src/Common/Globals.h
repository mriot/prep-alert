#pragma once

#include "Types.h"
#include <mumble/Mumble.h>
#include <nexus/Nexus.h>
#include <unordered_map>
#include <unordered_set>

namespace G
{
    constexpr const char *ADDON_NAME = "PrepAlert";

    extern HMODULE ModuleHandle;

    extern AddonAPI_t *APIDefs;
    extern NexusLinkData_t *NexusLink;
    extern Mumble::Data *MumbleLink;
    extern Mumble::Identity *MumbleIdentity;

    extern uint32_t CurrentMapID;
    extern uint32_t CurrentSectorID;
    extern std::unordered_set<uint32_t> SupportedMaps;
    extern std::unordered_map<int, MapData> MapDataMap;

    extern bool IsOptionsPaneOpen;
} // namespace G

