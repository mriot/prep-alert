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
}

namespace WorldState
{
    extern bool IsOnSupportedMap;
    extern Continent CurrentContinent;
    extern MapDefinition CurrentMapData;
    extern uint32_t CurrentSectorID;
    extern int CurrentMapFloor;
    extern std::unordered_set<uint32_t> SupportedMaps;
    extern std::unordered_map<int, MapDefinition> MapDataMap;
}

namespace UIState
{
    // set to true when options pane is open and reset to false each frame after overlay render
    // helps detect whether options pane is currently open
    extern bool IsOptionsPaneOpen;
}
