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
    j.at("name").get_to(b.name);
}

// Buffs
void from_json(const json &j, Buffs &b)
{
    if (j.contains("food") && !j["food"].is_null())
        b.food = j["food"].get<Buff>();
    else
        b.food.reset();

    if (j.contains("utility") && !j["utility"].is_null())
        b.utility = j["utility"].get<Buff>();
    else
        b.utility.reset();

    if (j.contains("sigil") && !j["sigil"].is_null())
        b.sigil = j["sigil"].get<Buff>();
    else
        b.sigil.reset();
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
void from_json(const json &data, MapData &map_data)
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
        Log::Critical(std::format("from_json(MapData) failed: {}", e.what()).c_str());
    }
}

// Load raw bytes of resource with given ID and type RT_RCDATA
static std::optional<std::vector<char>> LoadResourceBytes(HMODULE hModule, int resourceId)
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
    return std::vector<char>(bytes, bytes + size);
}

std::unordered_map<int, MapData> LoadMapData()
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
        Log::Critical(std::format("Failed to parse JSON: {}", e.what()).c_str());
        return {};
    }

    // maps "map_id -> MapData"
    std::unordered_map<int, MapData> mapData;
    for (auto &[key, value] : data.items())
    {
        int id      = std::stoi(key);
        mapData[id] = value.get<MapData>();
    }

    return mapData;
}

// Load from JSON file
/*
 std::unordered_map<int, MapData> LoadMapData()
 {

     std::filesystem::path path = G::APIDefs->Paths_GetAddonDirectory(G::ADDON_NAME);
     std::filesystem::create_directories(path);

     path /= "maps.json";
     Log::Info(std::format("Loading config from {}", path.string()).c_str());

     std::ifstream file(path);
     if (!file.is_open())
     {
         Log::Critical("Failed to open json file!");
         return {};
     }

     json data;
     try
     {
         file >> data;
         file.close();
     }
     catch (const std::exception &e)
     {
         Log::Critical(std::format("Failed to parse json file: {}", e.what()).c_str());
         return {};
     }

     // maps map_id : MapData
     std::unordered_map<int, MapData> result;
     for (auto &[key, value] : data.items())
     {
         int id     = std::stoi(key);
         result[id] = value.get<MapData>();
     }

     return result;
 }
 */
