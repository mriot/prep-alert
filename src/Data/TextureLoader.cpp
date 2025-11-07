#include "TextureLoader.h"
#include "../../resource.h"
#include <Common/Globals.h>
#include <Common/Utils.h>
#include <format>
#include <nexus/Nexus.h>
#include <string>
#include <unordered_map>

struct ResourceInfo
{
    int resourceID;
    std::string resourceName;
};

// Buff ID -> resource ID (resource IDs are defined in resource.h)
std::unordered_map<int, ResourceInfo> BuffIconResources = {
    {9963, {SHARPENING_STONE, "SHARPENING_STONE"}},
    {9949, {DREDGE_SLAYING, "DREDGE_SLAYING"}},
    {9925, {FLAME_LEGION_SLAYING, "FLAME_LEGION_SLAYING"}},
    {8493, {GHOST_SLAYING, "GHOST_SLAYING"}},
    {9917, {INQUEST_SLAYING, "INQUEST_SLAYING"}},
    {9941, {NIGHTMARE_COURT_SLAYING, "NIGHTMARE_COURT_SLAYING"}},
    {9933, {OUTLAW_SLAYING, "OUTLAW_SLAYING"}},
    {9837, {UNDEAD_SLAYING, "UNDEAD_SLAYING"}},
    {9901, {DEMON_SLAYING, "DEMON_SLAYING"}},
    {23228, {DEMON_SLAYING, "DEMON_SLAYING"}}, // Scarlet's Armies Slaying shares the same icon
};

Texture_t *LoadTexture(int buffID)
{
    if (!BuffIconResources.contains(buffID))
    {
        Log::Warning(std::format("Buff ID {} not found in resources!", buffID).c_str());
        return nullptr;
    }

    ResourceInfo resInfo = BuffIconResources.at(buffID);
    return G::APIDefs->Textures_GetOrCreateFromResource(resInfo.resourceName.c_str(), resInfo.resourceID, G::ModuleHandle);
}
