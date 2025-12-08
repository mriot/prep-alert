#pragma once

#include "imgui/imgui.h"

#include <optional>
#include <string>
#include <vector>


/// ----------------------------------------------------------------------------------------------------
/// GENERAL TYPES
/// ----------------------------------------------------------------------------------------------------

struct Vec2
{
    double x;
    double y;
};

enum class Pivot : std::uint8_t
{
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
};


/// ----------------------------------------------------------------------------------------------------
/// MAP / BUFF DATA TYPES
/// ----------------------------------------------------------------------------------------------------

enum class Continent : std::uint8_t
{
    Tyria,
    Mists,
    Unknown
};

// Important: must match PYTHON enum
enum class BuffType : std::uint8_t
{
    Utility,
    NightSigil,
    SlayingSigil,
};

struct Buff
{
    int id;
    BuffType type;
    bool remove;
};

struct Buffs
{
    std::optional<Buff> utility;
    std::optional<Buff> sigilNight;
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

struct MapDefinition
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
    bool utility;
    bool nightSigil;
    bool slayingSigil;
};

struct Reminders
{
    MapTypeReminder dungeons;
    MapTypeReminder fractals;
};

struct Settings
{
    ImVec2 position;
    Pivot window_anchor{};
    Pivot overlay_origin{};
    bool anchorOriginSync{};
    bool compact{};
    bool tooltips{};
    bool horizontal{};
    bool iconFirst{};
    int flashDuration{};
    int imageSize{};
    Reminders reminders{};
};
