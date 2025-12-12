#pragma once

#include "Types.h"
#include <mumble/Mumble.h>
#include <nexus/Nexus.h>
#include <unordered_map>
#include <unordered_set>

namespace G
{
    constexpr const char *ADDON_NAME = "PrepAlert";

    inline HMODULE ModuleHandle = nullptr;

    inline AddonAPI_t *APIDefs              = nullptr;
    inline NexusLinkData_t *NexusLink       = nullptr;
    inline Mumble::Data *MumbleLink         = nullptr;
    inline Mumble::Identity *MumbleIdentity = nullptr;
}

namespace WorldState
{
    inline bool IsOnSupportedMap        = false;
    inline Continent CurrentContinent   = Continent::Unknown;
    inline MapDefinition CurrentMapData = {};
    inline uint32_t CurrentSectorID     = 0;
    inline int CurrentMapFloor          = 0;

    inline std::unordered_set<uint32_t> SupportedMaps;
    inline std::unordered_map<int, MapDefinition> MapDataMap;
}

namespace UIState
{
    // set to true when options pane is open and reset to false each frame after overlay render
    // helps detect whether options pane is currently open
    inline bool IsOptionsPaneOpen = false;
}
