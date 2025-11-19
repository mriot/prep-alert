#pragma once

#include <resource.h>
#include <string>
#include <unordered_map>

struct BuffInfo
{
    int resourceID;
    std::string name;
};

inline std::unordered_map<int, BuffInfo> BuffDefs = {
    {-1, {GENERIC_ENHANCEMENT, "ENHANCEMENT"}},
    {-2, {GENERIC_SIGIL, "SIGIL"}},
    {-3, {GENERIC_FOOD, "FOOD"}},

    // misc
    {9963, {SHARPENING_STONE, "SHARPENING_STONE"}},

    // slaying
    {9949, {DREDGE_SLAYING, "DREDGE_SLAYING"}},
    {9925, {FLAME_LEGION_SLAYING, "FLAME_LEGION_SLAYING"}},
    {8493, {GHOST_SLAYING, "GHOST_SLAYING"}},
    {9917, {INQUEST_SLAYING, "INQUEST_SLAYING"}},
    {9941, {NIGHTMARE_COURT_SLAYING, "NIGHTMARE_COURT_SLAYING"}},
    {9933, {OUTLAW_SLAYING, "OUTLAW_SLAYING"}},
    {9837, {UNDEAD_SLAYING, "UNDEAD_SLAYING"}},
    {9861, {ICE_BROOD_SLAYING, "ICE_BROOD_SLAYING"}},
    {9869, {DESTROYER_SLAYING, "DESTROYER_SLAYING"}},
    {9893, {ELEMENTAL_SLAYING, "ELEMENTAL_SLAYING"}},
    {9901, {DEMON_SLAYING, "DEMON_SLAYING"}},
    {9909, {SONS_OF_SVANIR_SLAYING, "SONS_OF_SVANIR_SLAYING"}},
    {23228, {DEMON_SLAYING, "SCARLETS_ARMIES_SLAYING"}}, // shares icon with demon slaying

    // sigils
    {15268, {NIGHT_SIGIL, "NIGHT_SIGIL"}},
    {9347, {SERPENT_SIGIL, "SERPENT_SIGIL"}},
    {9395, {DEMON_SIGIL, "DEMON_SIGIL"}},
};

