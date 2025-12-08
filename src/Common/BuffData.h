#pragma once

#include "Constants.h"
#include "Types.h"

#include <Resource.h>
#include <string>
#include <unordered_map>


namespace BuffData
{
    struct BuffInfo
    {
        int resourceID;
        std::string name;
        BuffType type;
    };

    // clang-format off
    inline const std::unordered_map<int, BuffInfo> BuffDefs = {
        { BuffIds::REMOVE_ENHANCEMENT,        { .resourceID = RC_GENERIC_ENHANCEMENT,       .name = "Potion of Calibration",    .type = BuffType::Utility       } },
        { BuffIds::REMOVE_SIGIL,              { .resourceID = RC_GENERIC_SIGIL,             .name = "Slaying Sigil",            .type = BuffType::SlayingSigil  } },

        // Utility
        { BuffIds::UNDEAD_SLAYING,             { .resourceID = RC_UNDEAD_SLAYING,            .name = "Undead Potion",            .type = BuffType::Utility       } },
        { BuffIds::ICEBROOD_SLAYING,           { .resourceID = RC_ICEBROOD_SLAYING,          .name = "Icebrood Potion",          .type = BuffType::Utility       } },
        { BuffIds::DESTROYER_SLAYING,          { .resourceID = RC_DESTROYER_SLAYING,         .name = "Destroyer Potion",         .type = BuffType::Utility       } },
        { BuffIds::ELEMENTAL_SLAYING,          { .resourceID = RC_ELEMENTAL_SLAYING,         .name = "Elemental Potion",         .type = BuffType::Utility       } },
        { BuffIds::DEMON_SLAYING,              { .resourceID = RC_DEMON_SLAYING,             .name = "Demon Potion",             .type = BuffType::Utility       } },
        { BuffIds::SONS_OF_SVANIR_SLAYING,     { .resourceID = RC_SONS_OF_SVANIR_SLAYING,    .name = "Sons of Svanir Potion",    .type = BuffType::Utility       } },
        { BuffIds::INQUEST_SLAYING,            { .resourceID = RC_INQUEST_SLAYING,           .name = "Inquest Potion",           .type = BuffType::Utility       } },
        { BuffIds::FLAME_LEGION_SLAYING,       { .resourceID = RC_FLAME_LEGION_SLAYING,      .name = "Flame Legion Potion",      .type = BuffType::Utility       } },
        { BuffIds::OUTLAW_SLAYING,             { .resourceID = RC_OUTLAW_SLAYING,            .name = "Outlaw Potion",            .type = BuffType::Utility       } },
        { BuffIds::NIGHTMARE_COURT_SLAYING,    { .resourceID = RC_NIGHTMARE_COURT_SLAYING,   .name = "Nightmare Court Potion",   .type = BuffType::Utility       } },
        { BuffIds::DREDGE_SLAYING,             { .resourceID = RC_DREDGE_SLAYING,            .name = "Dredge Potion",            .type = BuffType::Utility       } },
        { BuffIds::SCARLETS_ARMIES_SLAYING,    { .resourceID = RC_DEMON_SLAYING,             .name = "Scarlet's Armies Potion",  .type = BuffType::Utility       } }, // shares icon with demon slaying
        { BuffIds::GHOST_SLAYING,              { .resourceID = RC_GHOST_SLAYING,             .name = "Ghost Potion",             .type = BuffType::Utility       } },

        // Sigil
        { BuffIds::NIGHT_SIGIL,                { .resourceID = RC_NIGHT_SIGIL,               .name = "Night Sigil",              .type = BuffType::NightSigil    } },
        { BuffIds::SERPENT_SIGIL,              { .resourceID = RC_SERPENT_SIGIL,             .name = "Serpent Sigil",            .type = BuffType::SlayingSigil  } },
        { BuffIds::ICEBROOD_SIGIL,             { .resourceID = RC_ICEBROOD_SIGIL,            .name = "Icebrood Sigil",           .type = BuffType::SlayingSigil  } },
        { BuffIds::DEMON_SIGIL,                { .resourceID = RC_DEMON_SIGIL,               .name = "Demon Sigil",              .type = BuffType::SlayingSigil  } },
        { BuffIds::DREDGE_SIGIL,               { .resourceID = RC_DREDGE_SIGIL,              .name = "Dredge Sigil",             .type = BuffType::SlayingSigil  } },
        { BuffIds::GHOST_SIGIL,                { .resourceID = RC_GHOST_SIGIL,               .name = "Ghost Sigil",              .type = BuffType::SlayingSigil  } },
        { BuffIds::OUTLAW_SIGIL,               { .resourceID = RC_OUTLAW_SIGIL,              .name = "Outlaw Sigil",             .type = BuffType::SlayingSigil  } },
        { BuffIds::NIGHTMARE_COURT_SIGIL,      { .resourceID = RC_NIGHTMARE_COURT_SIGIL,     .name = "Nightmare Court Sigil",    .type = BuffType::SlayingSigil  } },
        { BuffIds::INQUEST_SIGIL,              { .resourceID = RC_INQUEST_SIGIL,             .name = "Inquest Sigil",            .type = BuffType::SlayingSigil  } },
        { BuffIds::DESTROYER_SIGIL,            { .resourceID = RC_DESTROYER_SIGIL,           .name = "Destroyer Sigil",          .type = BuffType::SlayingSigil  } },
        { BuffIds::FLAME_LEGION_SIGIL,         { .resourceID = RC_FLAME_LEGION_SIGIL,        .name = "Flame Legion Sigil",       .type = BuffType::SlayingSigil  } },
        { BuffIds::SONS_OF_SVANIR_SIGIL,       { .resourceID = RC_SONS_OF_SVANIR_SIGIL,      .name = "Sons of Svanir Sigil",     .type = BuffType::SlayingSigil  } },
        { BuffIds::UNDEAD_SIGIL,               { .resourceID = RC_UNDEAD_SIGIL,              .name = "Undead Sigil",             .type = BuffType::SlayingSigil  } },
        { BuffIds::ELEMENTAL_SIGIL,            { .resourceID = RC_ELEMENTAL_SIGIL,           .name = "Elemental Sigil",          .type = BuffType::SlayingSigil  } },
    };
    // clang-format on

    inline const BuffInfo *GetBuffInfo(const int id) noexcept
    {
        const auto it = BuffDefs.find(id);
        return it != BuffDefs.end() ? &it->second : nullptr;
    }

    inline std::string GetBuffName(const int id) noexcept
    {
        const auto *buffInfo = GetBuffInfo(id);
        return buffInfo ? buffInfo->name : "[Unknown]";
    }
}
