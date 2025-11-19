#include "Globals.h"
#include <mumble/Mumble.h>
#include <nexus/Nexus.h>

namespace G
{
    HMODULE ModuleHandle = nullptr;

    AddonAPI_t *APIDefs              = nullptr;
    NexusLinkData_t *NexusLink       = nullptr;
    Mumble::Data *MumbleLink         = nullptr;
    Mumble::Identity *MumbleIdentity = nullptr;

    bool IsOnSupportedMap    = false;
    uint32_t CurrentMapID    = 0;
    uint32_t CurrentSectorID = 0;
    int CurrentMapFloor      = 0;
    std::unordered_set<uint32_t> SupportedMaps;
    std::unordered_map<int, MapData> MapDataMap;
}

namespace UIState
{
    bool IsOptionsPaneOpen = false;
}
