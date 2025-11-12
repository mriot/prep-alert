#pragma once

#include <optional>
#include <string>
#include <vector>

struct Vec2
{
    double x;
    double y;
};

struct Buff
{
    int id;
    std::string name;
};

struct Buffs
{
    std::optional<Buff> food;
    std::optional<Buff> utility;
    std::optional<Buff> sigil;
};

struct Sector
{
    int id;
    std::string name;
    std::vector<Vec2> bounds;
    Buffs buffs;
};

struct MapData
{
    int id;
    std::vector<Vec2> map_rect;
    std::string name;
    std::vector<Sector> sectors;
    Buffs default_buffs;
};

// -- SETTINGS --

struct ShownBuffTypes
{
    bool food;
    bool utility;
    bool sigil;
};

struct Position
{
    float x;
    float y;
};

struct Settings
{
    Position position;
    bool compact;
    bool tooltips;
    bool horizontal;
    int flashDuration;
    int imageSize;
    ShownBuffTypes shownBuffTypes;
};
