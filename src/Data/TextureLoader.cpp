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
        Log::Warning(std::format("Buff ID {} not found in resources!", buffID).c_str());
        return nullptr;
    }

    BuffInfo resInfo = BuffDefs.at(buffID);
    return G::APIDefs->Textures_GetOrCreateFromResource(resInfo.name.c_str(), resInfo.resourceID, G::ModuleHandle);
}
