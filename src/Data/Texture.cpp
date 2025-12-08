#include "Texture.h"

#include <algorithm>
#include <Common/BuffData.h>
#include <Common/Globals.h>
#include <Common/Utils.h>
#include <format>
#include <nexus/Nexus.h>
#include <string>
#include <unordered_map>

namespace Texture
{
    namespace
    {
        std::string ToSlug(std::string str)
        {
            std::ranges::replace(str, ' ', '_');
            std::ranges::transform(str, str.begin(),
                                   [](const unsigned char chr) { return std::toupper(chr); });
            return str;
        }
    }

    Texture_t *LoadBuffTex(int buffID)
    {
        if (!BuffData::BuffDefs.contains(buffID))
        {
            Log::Warning(std::format("Buff ID {} not found in resources!", buffID));
            return nullptr;
        }

        auto [resourceID, name, type] = BuffData::BuffDefs.at(buffID);
        return G::APIDefs->Textures_GetOrCreateFromResource(std::format("{}_{}", G::ADDON_NAME, ToSlug(name)).c_str(), resourceID, G::ModuleHandle);
    }
}
