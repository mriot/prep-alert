# ---------------------------------------------------------------------------- #
#                                  DATACLASSES                                 #
# ---------------------------------------------------------------------------- #
from dataclasses import dataclass
from enum import Enum
from typing import List


@dataclass
class Buff:
    id: int
    name: str
    icon: str


@dataclass
class Buffs:
    utility: Buff | None = None
    sigil: Buff | None = None


@dataclass
class SectorPatch:
    sector_id: int
    buffs: Buffs | None = None
    name: str | None = None
    bounds: List[List[float]] | None = None


@dataclass
class NewSector:
    sector_id: int
    name: str
    buffs: Buffs
    bounds: List[List[float]]


@dataclass
class MapPatch:
    map_id: int
    default: Buffs
    sector_patches: List[SectorPatch | NewSector]


# ---------------------------------------------------------------------------- #
#                                  DUNGEON MAP                                 #
# ---------------------------------------------------------------------------- #


class DungeonMap(int, Enum):
    # Ascalonian Catacombs
    AC_STORY = 33
    AC_EXPLORABLE = 36

    # Caudecus's Manor
    CM_STORY = 75
    CM_EXPLORABLE = 76

    # Twilight Arbor (NOTE: ids are reversed)
    TA_STORY = 68
    TA_EXPLORABLE = 67

    # Sorrow’s Embrace
    SE_STORY = 63
    SE_EXPLORABLE = 64

    # Citadel of Flame
    CoF_STORY = 66
    CoF_EXPLORABLE = 69

    # Honor of the Waves
    HotW_STORY = 70
    HotW_EXPLORABLE = 71

    # Crucible of Eternity
    CoE_STORY = 81
    CoE_EXPLORABLE = 82

    # Arah (no story)
    ARAH_EXPLORABLE = 112


# ---------------------------------------------------------------------------- #
#                                  FRACTAL MAP                                 #
# ---------------------------------------------------------------------------- #


class FractalMap(int, Enum):
    LONELY_TOWER = 1538
    SILENT_SURF = 1500
    SUNQUA_PEAK = 1384
    SHATTERED_OBSERVATORY = 1205
    NIGHTMARE = 1177
    KINFALL = 1584
