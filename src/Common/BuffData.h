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
    {BuffIds::UNDEAD_SLAYING, {RC_UNDEAD_SLAYING, "UNDEAD_SLAYING", BuffType::Utility}},
    {BuffIds::ICEBROOD_SLAYING, {RC_ICEBROOD_SLAYING, "ICEBROOD_SLAYING", BuffType::Utility}},
    {BuffIds::DESTROYER_SLAYING, {RC_DESTROYER_SLAYING, "DESTROYER_SLAYING", BuffType::Utility}},
    {BuffIds::ELEMENTAL_SLAYING, {RC_ELEMENTAL_SLAYING, "ELEMENTAL_SLAYING", BuffType::Utility}},
    {BuffIds::DEMON_SLAYING, {RC_DEMON_SLAYING, "DEMON_SLAYING", BuffType::Utility}},
    {BuffIds::SONS_OF_SVANIR_SLAYING, {RC_SONS_OF_SVANIR_SLAYING, "SONS_OF_SVANIR_SLAYING", BuffType::Utility}},
    {BuffIds::INQUEST_SLAYING, {RC_INQUEST_SLAYING, "INQUEST_SLAYING", BuffType::Utility}},
    {BuffIds::FLAME_LEGION_SLAYING, {RC_FLAME_LEGION_SLAYING, "FLAME_LEGION_SLAYING", BuffType::Utility}},
    {BuffIds::OUTLAW_SLAYING, {RC_OUTLAW_SLAYING, "OUTLAW_SLAYING", BuffType::Utility}},
    {BuffIds::NIGHTMARE_COURT_SLAYING, {RC_NIGHTMARE_COURT_SLAYING, "NIGHTMARE_COURT_SLAYING", BuffType::Utility}},
    {BuffIds::DREDGE_SLAYING, {RC_DREDGE_SLAYING, "DREDGE_SLAYING", BuffType::Utility}},
    {BuffIds::SCARLETS_ARMIES_SLAYING, {RC_DEMON_SLAYING, "SCARLETS_ARMIES_SLAYING", BuffType::Utility}}, // shares icon with demon slaying
    {BuffIds::GHOST_SLAYING, {RC_GHOST_SLAYING, "GHOST_SLAYING", BuffType::Utility}},

    // Sigil
    {BuffIds::GENERIC_SIGIL, {RC_GENERIC_SIGIL, "SIGIL", BuffType::Sigil}},
    {BuffIds::NIGHT_SIGIL, {RC_NIGHT_SIGIL, "NIGHT_SIGIL", BuffType::Sigil}},
    {BuffIds::SERPENT_SIGIL, {RC_SERPENT_SIGIL, "SERPENT_SIGIL", BuffType::Sigil}},
    {BuffIds::ICEBROOD_SIGIL, {RC_ICEBROOD_SIGIL, "ICEBROOD_SIGIL", BuffType::Sigil}},
    {BuffIds::DEMON_SIGIL, {RC_DEMON_SIGIL, "DEMON_SIGIL", BuffType::Sigil}},
    {BuffIds::DREDGE_SIGIL, {RC_DREDGE_SIGIL, "DREDGE_SIGIL", BuffType::Sigil}},
    {BuffIds::GHOST_SIGIL, {RC_GHOST_SIGIL, "GHOST_SIGIL", BuffType::Sigil}},
    {BuffIds::OUTLAW_SIGIL, {RC_OUTLAW_SIGIL, "OUTLAW_SIGIL", BuffType::Sigil}},
    {BuffIds::NIGHTMARE_COURT_SIGIL, {RC_NIGHTMARE_COURT_SIGIL, "NIGHTMARE_COURT_SIGIL", BuffType::Sigil}},
    {BuffIds::INQUEST_SIGIL, {RC_INQUEST_SIGIL, "INQUEST_SIGIL", BuffType::Sigil}},
    {BuffIds::DESTROYER_SIGIL, {RC_DESTROYER_SIGIL, "DESTROYER_SIGIL", BuffType::Sigil}},
    {BuffIds::FLAME_LEGION_SIGIL, {RC_FLAME_LEGION_SIGIL, "FLAME_LEGION_SIGIL", BuffType::Sigil}},
    {BuffIds::SONS_OF_SVANIR_SIGIL, {RC_SONS_OF_SVANIR_SIGIL, "SONS_OF_SVANIR_SIGIL", BuffType::Sigil}},
    {BuffIds::UNDEAD_SIGIL, {RC_UNDEAD_SIGIL, "UNDEAD_SIGIL", BuffType::Sigil}},
};
