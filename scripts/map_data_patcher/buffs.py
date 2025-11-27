from models import Buff, BuffType

GENERIC_ENHANCEMENT = Buff(-1, "Default Enhancement", BuffType.UTILITY)
GENERIC_SIGIL = Buff(-2, "Default Sigil", BuffType.SIGIL)
GENERIC_FOOD = Buff(-3, "Default Food", BuffType.FOOD)

# ENHANCEMENTS
UNDEAD_SLAYING = Buff(9837, "Potion of Undead Slaying", BuffType.UTILITY)
ICEBROOD_SLAYING = Buff(9861, "Potion of Icebrood Slaying", BuffType.UTILITY)
DESTROYER_SLAYING = Buff(9869, "Potion of Destroyer Slaying", BuffType.UTILITY)
ELEMENTAL_SLAYING = Buff(9893, "Potion of Elemental Slaying", BuffType.UTILITY)
DEMON_SLAYING = Buff(9901, "Potion of Demon Slaying", BuffType.UTILITY)
SONS_OF_SVANIR_SLAYING = Buff(9909, "Potion of Sons of Svanir Slaying", BuffType.UTILITY)
INQUEST_SLAYING = Buff(9917, "Potion of Inquest Slaying", BuffType.UTILITY)
FLAME_LEGION_SLAYING = Buff(9925, "Potion of Flame Legion Slaying", BuffType.UTILITY)
OUTLAW_SLAYING = Buff(9933, "Potion of Outlaw Slaying", BuffType.UTILITY)
NIGHTMARE_COURT_SLAYING = Buff(9941, "Potion of Nightmare Court Slaying", BuffType.UTILITY)
DREDGE_SLAYING = Buff(9949, "Potion of Dredge Slaying", BuffType.UTILITY)
SCARLETS_ARMIES_SLAYING = Buff(23228, "Potion of Slaying Scarlet's Armies", BuffType.UTILITY)
GHOST_SLAYING = Buff(8493, "Potion of Ghost Slaying", BuffType.UTILITY)


# SIGILS
NIGHT_SIGIL = Buff(15268, "Night Sigil", BuffType.SIGIL)
NOT_NIGHT_SIGIL = Buff(15268, "Remove Night Sigil", BuffType.RESET)

SERPENT_SIGIL = Buff(9347, "Serpent Slaying Sigil", BuffType.SIGIL)
ICEBROOD_SIGIL = Buff(9340, "Icebrood Sigil", BuffType.SIGIL)
DEMON_SIGIL = Buff(9353, "Demon Sigil", BuffType.SIGIL)
DREDGE_SIGIL = Buff(9373, "Dredge Sigil", type=BuffType.SIGIL)
GHOST_SIGIL = Buff(9701, "Ghost Sigil", type=BuffType.SIGIL)
OUTLAW_SIGIL = Buff(9365, "Outlaw Sigil", type=BuffType.SIGIL)
NIGHTMARE_COURT_SIGIL = Buff(9370, "Nightmare Court Sigil", type=BuffType.SIGIL)
INQUEST_SIGIL = Buff(9359, "Inquest Sigil", type=BuffType.SIGIL)
DESTORYER_SIGIL = Buff(9343, "Destroyer Sigil", type=BuffType.SIGIL)
FLAME_LEGION_SIGIL = Buff(9362, "Flame Legion Sigil", type=BuffType.SIGIL)
SONS_OF_SVANIR_SIGIL = Buff(9356, "Sons of Svanir Sigil", type=BuffType.SIGIL)
UNDEAD_SIGIL = Buff(9331, "Undead Sigil", type=BuffType.SIGIL)
ELEMENTAL_SIGIL = Buff(9352, "Elemental Sigil", type=BuffType.SIGIL)
