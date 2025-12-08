#include "Globals.h"
#include <mumble/Mumble.h>
#include <nexus/Nexus.h>
#include "Types.h"

namespace G
{
    HMODULE ModuleHandle = nullptr;

    AddonAPI_t *APIDefs              = nullptr;
    NexusLinkData_t *NexusLink       = nullptr;
    Mumble::Data *MumbleLink         = nullptr;
    Mumble::Identity *MumbleIdentity = nullptr;
}

namespace WorldState
{
    bool IsOnSupportedMap        = false;
    Continent CurrentContinent   = Continent::Unknown;
    MapDefinition CurrentMapData = {};
    uint32_t CurrentSectorID     = 0;
    int CurrentMapFloor          = 0;
    std::unordered_set<uint32_t> SupportedMaps;
    std::unordered_map<int, MapDefinition> MapDataMap;
}

namespace UIState
{
    // set to true when options pane is open and reset to false each frame after overlay render
    // helps detect whether options pane is currently open
    bool IsOptionsPaneOpen = false;
}
s