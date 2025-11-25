#pragma once

#include "Constants.h"
#include "Types.h"

#include <Resource.h>
#include <string>
#include <unordered_map>

struct BuffInfo
{
    int resourceID;
    std::string name; // for texture naming (prefixed with addon name)
    BuffType type;
};

inline std::unordered_map<int, BuffInfo> BuffDefs = {
    // Food
    {BuffIds::GENERIC_FOOD, {RC_GENERIC_FOOD, "FOOD", BuffType::Food}},

    // Utility
    {BuffIds::GENERIC_ENHANCEMENT, {RC_GENERIC_ENHANCEMENT, "ENHANCEMENT", BuffType::Utility}},
    // {BuffIds::SHARPENING_STONE, {RC_SHARPENING_STONE, "SHARPENING_STONE", BuffType::Utility}},
    {BuffIds::DREDGE_SLAYING, {RC_DREDGE_SLAYING, "DREDGE_SLAYING", BuffType::Utility}},
    {BuffIds::FLAME_LEGION_SLAYING, {RC_FLAME_LEGION_SLAYING, "FLAME_LEGION_SLAYING", BuffType::Utility}},
    {BuffIds::GHOST_SLAYING, {RC_GHOST_SLAYING, "GHOST_SLAYING", BuffType::Utility}},
    {BuffIds::INQUEST_SLAYING, {RC_INQUEST_SLAYING, "INQUEST_SLAYING", BuffType::Utility}},
    {BuffIds::NIGHTMARE_COURT_SLAYING, {RC_NIGHTMARE_COURT_SLAYING, "NIGHTMARE_COURT_SLAYING", BuffType::Utility}},
    {BuffIds::OUTLAW_SLAYING, {RC_OUTLAW_SLAYING, "OUTLAW_SLAYING", BuffType::Utility}},
    {BuffIds::UNDEAD_SLAYING, {RC_UNDEAD_SLAYING, "UNDEAD_SLAYING", BuffType::Utility}},
    {BuffIds::ICE_BROOD_SLAYING, {RC_ICE_BROOD_SLAYING, "ICE_BROOD_SLAYING", BuffType::Utility}},
    {BuffIds::DESTROYER_SLAYING, {RC_DESTROYER_SLAYING, "DESTROYER_SLAYING", BuffType::Utility}},
    {BuffIds::ELEMENTAL_SLAYING, {RC_ELEMENTAL_SLAYING, "ELEMENTAL_SLAYING", BuffType::Utility}},
    {BuffIds::DEMON_SLAYING, {RC_DEMON_SLAYING, "DEMON_SLAYING", BuffType::Utility}},
    {BuffIds::SONS_OF_SVANIR_SLAYING, {RC_SONS_OF_SVANIR_SLAYING, "SONS_OF_SVANIR_SLAYING", BuffType::Utility}},
    {BuffIds::SCARLETS_ARMIES_SLAYING, {RC_DEMON_SLAYING, "SCARLETS_ARMIES_SLAYING", BuffType::Utility}}, // shares icon with demon slaying

    // Sigil
    {BuffIds::GENERIC_SIGIL, {RC_GENERIC_SIGIL, "SIGIL", BuffType::Sigil}},
    {BuffIds::NIGHT_SIGIL, {RC_NIGHT_SIGIL, "NIGHT_SIGIL", BuffType::Sigil}},
    {BuffIds::SERPENT_SIGIL, {RC_SERPENT_SIGIL, "SERPENT_SIGIL", BuffType::Sigil}},
    {BuffIds::DEMON_SIGIL, {RC_DEMON_SIGIL, "DEMON_SIGIL", BuffType::Sigil}},
};
