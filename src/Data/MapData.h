#pragma once

#include <unordered_map>
#include <Common/Types.h>

namespace MapData
{
    std::unordered_map<int, MapDefinition> Load();
}
