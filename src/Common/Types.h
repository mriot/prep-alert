#pragma once

#include "imgui/imgui.h"

#include <optional>
#include <string>
#include <vector>


/// ----------------------------------------------------------------------------------------------------
/// GENERAL TYPES
/// ----------------------------------------------------------------------------------------------------

// TODO remove and use ImVec2 directly?
struct Vec2
{
    double x;
    double y;
};

enum class Pivot : int
{
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
};

/// ----------------------------------------------------------------------------------------------------
/// MAP / BUFF DATA TYPES
/// ----------------------------------------------------------------------------------------------------

enum class Continent
{
    Tyria,
    Mists,
    Unknown
};

// do NOT change order - must match python enum
enum class BuffType
{
    Utility,
    Sigil,
    Food,
    Reset
};

struct Buff
{
    int id;
    std::string name;
    BuffType type;
};

struct Buffs
{
    std::optional<Buff> food;
    std::optional<Buff> utility;
    std::optional<Buff> sigil;
    std::optional<Buff> sigilSlaying;
};

struct Sector
{
    int id;
    std::string name;
    std::vector<Vec2> bounds;
    Buffs buffs;
    std::vector<int> floors;
};

struct MapData
{
    int id;
    int continent_id;
    std::vector<Vec2> map_rect;
    std::string name;
    std::vector<Sector> sectors;
    Buffs default_buffs;
};


///----------------------------------------------------------------------------------------------------
/// SETTINGS TYPES
///----------------------------------------------------------------------------------------------------

struct MapTypeReminder
{
    bool enabled;
    bool food;
    bool utility;
    bool sigil;
    bool sigilSlaying;
    bool defaultBuffs;
};

struct Reminders
{
    MapTypeReminder dungeons;
    MapTypeReminder fractals;
};

struct Settings
{
    ImVec2 position;
    Pivot window_anchor;
    Pivot overlay_origin;
    bool anchorOriginSync;
    bool compact;
    bool tooltips;
    bool horizontal;
    bool iconFirst;
    int flashDuration;
    int imageSize;
    Reminders reminders;
};
