#include "MapData.h"
#include "../Resource.h"
#include <Common/Globals.h>
#include <Common/Types.h>
#include <Common/Utils.h>
#include <exception>
#include <format>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

// 2D Vector
void from_json(const json &j, Vec2 &v)
{
    j.at(0).get_to(v.x);
    j.at(1).get_to(v.y);
}

// Buff
void from_json(const json &j, Buff &b)
{
    j.at("id").get_to(b.id);
    // j.at("name").get_to(b.name);
    j.at("type").get_to(b.type);
}

// Buffs
void from_json(const json &j, Buffs &b)
{
    if (j.contains("utility") && !j["utility"].is_null())
        b.utility = j["utility"].get<Buff>();
    else
        b.utility.reset();

    if (j.contains("sigil_night") && !j["sigil_night"].is_null())
        b.sigilNight = j["sigil_night"].get<Buff>();
    else
        b.sigilNight.reset();

    if (j.contains("sigil_slaying") && !j["sigil_slaying"].is_null())
        b.sigilSlaying = j["sigil_slaying"].get<Buff>();
    else
        b.sigilSlaying.reset();
}

// Sector
void from_json(const json &j, Sector &s)
{
    j.at("id").get_to(s.id);
    j.at("name").get_to(s.name);
    j.at("bounds").get_to(s.bounds);
    j.at("floors").get_to(s.floors);

    if (j.contains("buffs"))
        j.at("buffs").get_to(s.buffs);
}

// Map
void from_json(const json &data, MapDefinition &map_data)
{
    try
    {
        if (data.contains("id"))
            data.at("id").get_to(map_data.id);
        if (data.contains("continent_id"))
            data.at("continent_id").get_to(map_data.continent_id);
        if (data.contains("name"))
            data.at("name").get_to(map_data.name);
        if (data.contains("default_buffs"))
            data.at("default_buffs").get_to(map_data.default_buffs);
        if (data.contains("sectors"))
            data.at("sectors").get_to(map_data.sectors);
    }
    catch (const std::exception &e)
    {
        Log::Critical(std::format("from_json(MapData) failed: {}", e.what()));
    }
}

namespace MapData
{
    namespace
    {
        std::optional<std::vector<char>> LoadResourceBytes(const HMODULE hModule, int resourceId)
        {
            if (!hModule)
                return std::nullopt;

            const HRSRC hRes = FindResourceW(hModule, MAKEINTRESOURCEW(resourceId), L"JSON");

            if (!hRes)
            {
                Log::Warning("Didnt find resource");
                return std::nullopt;
            }

            const HGLOBAL hGlobal = LoadResource(hModule, hRes);
            if (!hGlobal)
            {
                Log::Warning("Unable to load");
                return std::nullopt;
            }

            const DWORD size = SizeofResource(hModule, hRes);
            if (size == 0)
            {
                Log::Warning("Size is 0");
                return std::nullopt;
            }

            const void *pData = LockResource(hGlobal);
            if (!pData)
            {
                Log::Warning("Could not lock");
                return std::nullopt;
            }

            const char *bytes = static_cast<const char *>(pData);
            return std::vector(bytes, bytes + size);
        }
    }

    std::unordered_map<int, MapDefinition> Load()
    {
        const std::optional<std::vector<char>> resourceBytes = LoadResourceBytes(G::ModuleHandle, RC_MAP_DATA);

        if (!resourceBytes)
        {
            Log::Critical("Failed to load resource bytes!");
            return {};
        }

        json data;
        try
        {
            data = json::parse(resourceBytes->begin(), resourceBytes->end());
        }
        catch (const std::exception &e)
        {
            Log::Critical(std::format("Failed to parse JSON: {}", e.what()));
            return {};
        }

        // maps "map_id -> MapData"
        std::unordered_map<int, MapDefinition> mapData;
        for (const auto &[key, value] : data.items())
        {
            int mapId      = std::stoi(key);
            mapData[mapId] = value.get<MapDefinition>();
        }

        return mapData;
    }
}
