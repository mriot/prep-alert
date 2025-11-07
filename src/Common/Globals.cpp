#include "Globals.h"
#include <mumble/Mumble.h>
#include <nexus/Nexus.h>

namespace G
{
    HMODULE ModuleHandle = nullptr;

    AddonAPI_t *APIDefs              = nullptr;
    Mumble::Data *MumbleLink         = nullptr;
    Mumble::Identity *MumbleIdentity = nullptr;
    NexusLinkData_t *NexusLink       = nullptr;

    uint32_t CurrentMapID    = 0;
    uint32_t CurrentSectorID = 0;
    std::unordered_set<uint32_t> SupportedMaps;
    std::unordered_map<int, MapData> MapDataMap;
} // namespace G
