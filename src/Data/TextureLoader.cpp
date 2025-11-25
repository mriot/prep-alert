#include "TextureLoader.h"
#include <Common/BuffData.h>
#include <Common/Globals.h>
#include <Common/Utils.h>
#include <format>
#include <nexus/Nexus.h>
#include <string>
#include <unordered_map>

Texture_t *LoadTexture(int buffID)
{
    if (!BuffDefs.contains(buffID))
    {
        Log::Warning(std::format("Buff ID {} not found in resources!", buffID));
        return nullptr;
    }

    auto [resourceID, name, type] = BuffDefs.at(buffID);
    return G::APIDefs->Textures_GetOrCreateFromResource((std::format("{}_{}", G::ADDON_NAME, name)).c_str(), resourceID, G::ModuleHandle);
}
