from buffs import (
    DEMON_SIGIL,
    DEMON_SLAYING,
    DESTORYER_SIGIL,
    DESTROYER_SLAYING,
    DREDGE_SIGIL,
    DREDGE_SLAYING,
    ELEMENTAL_SIGIL,
    ELEMENTAL_SLAYING,
    FLAME_LEGION_SIGIL,
    FLAME_LEGION_SLAYING,
    GENERIC_ENHANCEMENT,
    GENERIC_SIGIL,
    GHOST_SIGIL,
    GHOST_SLAYING,
    ICEBROOD_SIGIL,
    ICEBROOD_SLAYING,
    INQUEST_SIGIL,
    INQUEST_SLAYING,
    NIGHT_SIGIL,
    NIGHTMARE_COURT_SIGIL,
    NIGHTMARE_COURT_SLAYING,
    NOT_NIGHT_SIGIL,
    OUTLAW_SIGIL,
    OUTLAW_SLAYING,
    SCARLETS_ARMIES_SLAYING,
    SERPENT_SIGIL,
    SONS_OF_SVANIR_SIGIL,
    SONS_OF_SVANIR_SLAYING,
    UNDEAD_SIGIL,
    UNDEAD_SLAYING,
)
from models import Buffs, DungeonMap, FractalMap, MapPatch, NewSector, SectorPatch

PATCHES = [
    # --------------------------- LONELY TOWER FRACTAL --------------------------- #
    MapPatch(
        map_id=FractalMap.LONELY_TOWER,
        default=Buffs(
            utility=GENERIC_ENHANCEMENT, sigil=NOT_NIGHT_SIGIL, sigil_slaying=GENERIC_SIGIL
        ),
        floors=[63, 64],  # TODO not currently patched into map data (not really needed either)
        sector_patches=[
            SectorPatch(
                name="Lower floor",  # default floor 63
                sector_id=2016,
                bounds=[
                    [12048, 4650],
                    [12077, 5315],
                    [13181, 5321],
                    [13178, 4650],
                ],
            ),
            NewSector(
                sector_id=2017,
                name="Upper floor",
                buffs=Buffs(utility=DEMON_SLAYING, sigil_slaying=DEMON_SIGIL),
                floors=[64],  # this is a custom floor and handled in the addon
                bounds=[
                    [12048, 4650],
                    [12077, 5315],
                    [13181, 5321],
                    [13178, 4650],
                ],
            ),
        ],
    ),
    # --------------------------- SHATTERED OBSERVATORY -------------------------- #
    MapPatch(
        map_id=FractalMap.SHATTERED_OBSERVATORY,
        default=Buffs(
            utility=GENERIC_ENHANCEMENT, sigil=NOT_NIGHT_SIGIL, sigil_slaying=GENERIC_SIGIL
        ),
        sector_patches=[
            SectorPatch(
                sector_id=1478,
                buffs=Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil_slaying=INQUEST_SIGIL),
                bounds=[
                    [11348, 6063],
                    [11344, 6235],
                    [11729, 6236],
                    [11691, 6060],
                ],
            ),
        ],
    ),
    # ---------------------------- SILENT SURF FRACTAL --------------------------- #
    MapPatch(
        map_id=FractalMap.SILENT_SURF,
        default=Buffs(
            utility=GENERIC_ENHANCEMENT, sigil=NOT_NIGHT_SIGIL, sigil_slaying=GENERIC_SIGIL
        ),
        sector_patches=[],
    ),
    # ------------------------------ KINFALL FRACTAL ----------------------------- #
    MapPatch(
        map_id=FractalMap.KINFALL,
        default=Buffs(
            utility=ICEBROOD_SLAYING, sigil=NOT_NIGHT_SIGIL, sigil_slaying=ICEBROOD_SIGIL
        ),
        sector_patches=[],
    ),
    # ----------------------------- NIGHTMARE FRACTAL ---------------------------- #
    MapPatch(
        map_id=FractalMap.NIGHTMARE,
        default=Buffs(
            utility=SCARLETS_ARMIES_SLAYING, sigil=NOT_NIGHT_SIGIL, sigil_slaying=SERPENT_SIGIL
        ),
        sector_patches=[
            SectorPatch(
                sector_id=1412,
                name="Spawn + MAMA",
                buffs=Buffs(sigil_slaying=GENERIC_SIGIL),
                bounds=[
                    [11964, 4483],
                    [12172, 4461],
                    [12364, 4505],
                    [12245, 4632],
                    [12087, 4644],
                    [11960, 4665],
                ],
            ),
        ],
    ),
    # ---------------------------- SUNQUA PEAK FRACTAL --------------------------- #
    MapPatch(
        map_id=FractalMap.SUNQUA_PEAK,
        default=Buffs(
            utility=GENERIC_ENHANCEMENT, sigil=NOT_NIGHT_SIGIL, sigil_slaying=GENERIC_SIGIL
        ),
        sector_patches=[],
    ),
    # ---------------------------------------------------------------------------- #
    #                                   DUNGEONS                                   #
    # ---------------------------------------------------------------------------- #
    # ----------------------------- Ascalonian Catacombs ---------------------------- #
    MapPatch(
        DungeonMap.AC_STORY,
        Buffs(utility=GHOST_SLAYING, sigil=NIGHT_SIGIL, sigil_slaying=GHOST_SIGIL),
        [],
    ),
    MapPatch(
        DungeonMap.AC_EXPLORABLE,
        Buffs(utility=GENERIC_ENHANCEMENT, sigil=NIGHT_SIGIL, sigil_slaying=GENERIC_SIGIL),
        [
            # Kholer
            SectorPatch(9, Buffs(utility=GHOST_SLAYING, sigil_slaying=GHOST_SIGIL)),
        ],
    ),
    # ----------------------------- Caudecus's Manor ----------------------------- #
    MapPatch(
        DungeonMap.CM_STORY,
        Buffs(utility=OUTLAW_SLAYING, sigil=NOT_NIGHT_SIGIL, sigil_slaying=OUTLAW_SIGIL),
        [],
    ),
    MapPatch(
        DungeonMap.CM_EXPLORABLE,
        Buffs(utility=OUTLAW_SLAYING, sigil=NOT_NIGHT_SIGIL, sigil_slaying=OUTLAW_SIGIL),
        [],
    ),
    # ------------------------------ Twilight Arbor ------------------------------ #
    MapPatch(
        DungeonMap.TA_STORY,
        Buffs(
            utility=NIGHTMARE_COURT_SLAYING,
            sigil=NIGHT_SIGIL,
            sigil_slaying=NIGHTMARE_COURT_SIGIL,
        ),
        [],
    ),
    MapPatch(
        DungeonMap.TA_EXPLORABLE,
        Buffs(
            utility=NIGHTMARE_COURT_SLAYING,
            sigil=NIGHT_SIGIL,
            sigil_slaying=NIGHTMARE_COURT_SIGIL,
        ),
        [
            # Up/Fwd Endboss
            SectorPatch(1135, Buffs(utility=GENERIC_ENHANCEMENT, sigil_slaying=GENERIC_SIGIL)),
            # Aether
            SectorPatch(1129, Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil_slaying=GENERIC_SIGIL)),
            SectorPatch(1130, Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil_slaying=GENERIC_SIGIL)),
            SectorPatch(1131, Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil_slaying=GENERIC_SIGIL)),
            SectorPatch(1139, Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil_slaying=GENERIC_SIGIL)),
        ],
    ),
    # ----------------------------- Sorrow's Embrace ----------------------------- #
    MapPatch(
        DungeonMap.SE_STORY,
        Buffs(utility=DREDGE_SLAYING, sigil=NIGHT_SIGIL, sigil_slaying=DREDGE_SIGIL),
        [
            NewSector(
                199,
                "Inspector Snik",
                Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
                [
                    [52613, 34438],
                    [52611, 34544],
                    [52486, 34539],
                    [52438, 34430],
                ],
                floors=[-7],
            ),
            NewSector(
                200,
                "Advanced Assault Golem",
                Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
                [
                    [52609, 34433],
                    [52608, 34565],
                    [52869, 34565],
                    [52868, 34439],
                ],
                floors=[-7],
            ),
            NewSector(
                201,
                "Inquest Base",
                Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
                [
                    [53213, 34426],
                    [53216, 34651],
                    [53123, 34691],
                    [53271, 35086],
                    [53618, 34731],
                    [53605, 34405],
                ],
                floors=[-7],
            ),
            NewSector(
                202,
                "Forgeman",
                Buffs(utility=GENERIC_ENHANCEMENT, sigil_slaying=GENERIC_SIGIL),
                [
                    [52958, 34911],
                    [53298, 34870],
                    [53299, 35310],
                    [52921, 35310],
                ],
                floors=[-7],
            ),
        ],
    ),
    MapPatch(
        DungeonMap.SE_EXPLORABLE,
        Buffs(utility=DREDGE_SLAYING, sigil=NIGHT_SIGIL, sigil_slaying=DREDGE_SIGIL),
        [
            # p1
            SectorPatch(174, Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL)),
            SectorPatch(179, Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL)),
            SectorPatch(180, Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL)),
            SectorPatch(181, Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL)),
            # p3 cart
            SectorPatch(172, Buffs(utility=GENERIC_ENHANCEMENT, sigil_slaying=GENERIC_SIGIL)),
            # p3 endboss
            SectorPatch(173, Buffs(utility=DESTROYER_SLAYING, sigil_slaying=DESTORYER_SIGIL)),
        ],
    ),
    # ----------------------------- Citadel of Flame ----------------------------- #
    MapPatch(
        DungeonMap.CoF_STORY,
        Buffs(utility=FLAME_LEGION_SLAYING, sigil=NIGHT_SIGIL, sigil_slaying=FLAME_LEGION_SIGIL),
        [],
    ),
    MapPatch(
        DungeonMap.CoF_EXPLORABLE,
        Buffs(utility=FLAME_LEGION_SLAYING, sigil=NIGHT_SIGIL, sigil_slaying=FLAME_LEGION_SIGIL),
        [],
    ),
    # ---------------------------- Honor of the Waves ---------------------------- #
    MapPatch(
        DungeonMap.HotW_STORY,
        Buffs(
            utility=SONS_OF_SVANIR_SLAYING,
            sigil=NOT_NIGHT_SIGIL,
            sigil_slaying=SONS_OF_SVANIR_SIGIL,
        ),
        [
            # Entrance Waypoint (waiting for skip)
            SectorPatch(
                653,
                Buffs(
                    utility=ICEBROOD_SLAYING,
                    sigil_slaying=ICEBROOD_SIGIL,
                ),
                bounds=[
                    [55066, 25159],
                    [55068, 25157],
                    [55065, 25155],
                    [55064, 25157],
                ],
            ),
            # Honor's Voice
            SectorPatch(665, Buffs(utility=ICEBROOD_SLAYING, sigil_slaying=ICEBROOD_SIGIL)),
            # Kodan's Bane
            SectorPatch(666, Buffs(utility=ICEBROOD_SLAYING, sigil_slaying=ICEBROOD_SIGIL)),
        ],
    ),
    MapPatch(
        DungeonMap.HotW_EXPLORABLE,
        Buffs(utility=ICEBROOD_SLAYING, sigil=NOT_NIGHT_SIGIL, sigil_slaying=ICEBROOD_SIGIL),
        [
            # Aldus Stormbringer (p1)
            SectorPatch(
                658, Buffs(utility=SONS_OF_SVANIR_SLAYING, sigil_slaying=SONS_OF_SVANIR_SIGIL)
            )
        ],
    ),
    # --------------------------- Crucible of Eternity --------------------------- #
    MapPatch(
        DungeonMap.CoE_STORY,
        Buffs(utility=INQUEST_SLAYING, sigil=NOT_NIGHT_SIGIL, sigil_slaying=INQUEST_SIGIL),
        floors=[-11, -12],  # TODO not currently patched into map data (not really needed either)
        sector_patches=[
            NewSector(
                sector_id=2001,
                name="Kudu",
                floors=[-12],  # custom floor
                buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil_slaying=GENERIC_SIGIL),
                bounds=[
                    [53699, 38218],
                    [53695, 38324],
                    [53822, 38324],
                    [53818, 38215],
                ],
            )
        ],
    ),
    # TODO
    MapPatch(
        DungeonMap.CoE_EXPLORABLE,
        Buffs(utility=GENERIC_ENHANCEMENT, sigil=NOT_NIGHT_SIGIL, sigil_slaying=GENERIC_SIGIL),
        [],
    ),
    # ----------------------------------- Arah ----------------------------------- #
    MapPatch(
        DungeonMap.ARAH_EXPLORABLE,
        Buffs(utility=UNDEAD_SLAYING, sigil=NOT_NIGHT_SIGIL, sigil_slaying=UNDEAD_SIGIL),
        [
            # P1 Shoggroth
            SectorPatch(787, Buffs(utility=ELEMENTAL_SLAYING, sigil_slaying=ELEMENTAL_SIGIL)),
        ],
    ),
]
