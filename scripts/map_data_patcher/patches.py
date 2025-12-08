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
    GHOST_SIGIL,
    GHOST_SLAYING,
    ICEBROOD_SIGIL,
    ICEBROOD_SLAYING,
    INQUEST_SIGIL,
    INQUEST_SLAYING,
    NIGHT_SIGIL,
    NIGHTMARE_COURT_SIGIL,
    NIGHTMARE_COURT_SLAYING,
    NO_NIGHT_SIGIL,
    NO_SLAYING_POTION,
    NO_SLAYING_SIGIL,
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
            utility=NO_SLAYING_POTION,
            sigil_night=NO_NIGHT_SIGIL,
            sigil_slaying=NO_SLAYING_SIGIL,
        ),
        floors=[63, 64],
        sector_patches=[
            SectorPatch(
                name="Lower floor",  # default floor 63
                sector_id=2016,
            ),
            NewSector(
                sector_id=-1,
                name="Upper floor",
                buffs=Buffs(utility=DEMON_SLAYING, sigil_slaying=DEMON_SIGIL),
                floors=[64],  # custom floor
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
            utility=NO_SLAYING_POTION,
            sigil_night=NO_NIGHT_SIGIL,
            sigil_slaying=NO_SLAYING_SIGIL,
        ),
        sector_patches=[
            NewSector(
                sector_id=-1,
                name="Arkk",
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
            utility=NO_SLAYING_POTION,
            sigil_night=NO_NIGHT_SIGIL,
            sigil_slaying=NO_SLAYING_SIGIL,
        ),
        sector_patches=[],
    ),
    # ------------------------------ KINFALL FRACTAL ----------------------------- #
    MapPatch(
        map_id=FractalMap.KINFALL,
        default=Buffs(
            utility=ICEBROOD_SLAYING, sigil_night=NO_NIGHT_SIGIL, sigil_slaying=ICEBROOD_SIGIL
        ),
        sector_patches=[],
    ),
    # ----------------------------- NIGHTMARE FRACTAL ---------------------------- #
    MapPatch(
        map_id=FractalMap.NIGHTMARE,
        default=Buffs(
            utility=SCARLETS_ARMIES_SLAYING,
            sigil_night=NO_NIGHT_SIGIL,
            sigil_slaying=SERPENT_SIGIL,
        ),
        sector_patches=[
            NewSector(
                sector_id=-1,
                name="Spawn + MAMA",
                buffs=Buffs(sigil_slaying=NO_SLAYING_SIGIL),
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
            utility=NO_SLAYING_POTION,
            sigil_night=NO_NIGHT_SIGIL,
            sigil_slaying=NO_SLAYING_SIGIL,
        ),
        sector_patches=[],
    ),
    # ---------------------------------------------------------------------------- #
    #                                   DUNGEONS                                   #
    # ---------------------------------------------------------------------------- #
    # ----------------------------- Ascalonian Catacombs ---------------------------- #
    MapPatch(
        map_id=DungeonMap.AC_STORY,
        default=Buffs(utility=GHOST_SLAYING, sigil_night=NIGHT_SIGIL, sigil_slaying=GHOST_SIGIL),
        sector_patches=[],
    ),
    MapPatch(
        map_id=DungeonMap.AC_EXPLORABLE,
        default=Buffs(
            utility=NO_SLAYING_POTION,
            sigil_night=NIGHT_SIGIL,
            sigil_slaying=NO_SLAYING_SIGIL,
        ),
        sector_patches=[],
    ),
    # ----------------------------- Caudecus's Manor ----------------------------- #
    MapPatch(
        map_id=DungeonMap.CM_STORY,
        default=Buffs(
            utility=OUTLAW_SLAYING, sigil_night=NO_NIGHT_SIGIL, sigil_slaying=OUTLAW_SIGIL
        ),
        sector_patches=[],
    ),
    MapPatch(
        map_id=DungeonMap.CM_EXPLORABLE,
        default=Buffs(
            utility=OUTLAW_SLAYING, sigil_night=NO_NIGHT_SIGIL, sigil_slaying=OUTLAW_SIGIL
        ),
        sector_patches=[],
    ),
    # ------------------------------ Twilight Arbor ------------------------------ #
    MapPatch(
        map_id=DungeonMap.TA_STORY,
        default=Buffs(
            utility=NIGHTMARE_COURT_SLAYING,
            sigil_night=NIGHT_SIGIL,
            sigil_slaying=NIGHTMARE_COURT_SIGIL,
        ),
        sector_patches=[],
    ),
    MapPatch(
        map_id=DungeonMap.TA_EXPLORABLE,
        default=Buffs(
            utility=NIGHTMARE_COURT_SLAYING,
            sigil_night=NIGHT_SIGIL,
            sigil_slaying=NIGHTMARE_COURT_SIGIL,
        ),
        sector_patches=[
            # Up/Fwd Endboss
            # SectorPatch(1135, Buffs(utility=NO_SLAYING_POTION, sigil_slaying=NO_SLAYING_SIGIL)),
            NewSector(
                sector_id=-1,
                name="Up/Fwd Endboss",
                buffs=Buffs(utility=NO_SLAYING_POTION, sigil_slaying=NO_SLAYING_SIGIL),
                bounds=[
                    [42353, 32835],
                    [42291, 32877],
                    [42209, 32817],
                    [42125, 32923],
                    [42303, 33030],
                    [42333, 33009],
                    [42271, 32964],
                    [42349, 32893],
                    [42373, 32849],
                ],
            ),
            # Aether
            SectorPatch(
                1129, Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil_slaying=NO_SLAYING_SIGIL)
            ),
            SectorPatch(
                1130, Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil_slaying=NO_SLAYING_SIGIL)
            ),
            SectorPatch(
                1131, Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil_slaying=NO_SLAYING_SIGIL)
            ),
            SectorPatch(
                1139, Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil_slaying=NO_SLAYING_SIGIL)
            ),
            # Clockheart
            NewSector(
                sector_id=-2,
                name="Clockheart",
                buffs=Buffs(utility=NO_SLAYING_POTION, sigil_slaying=NO_SLAYING_SIGIL),
                bounds=[
                    [41682, 32384],
                    [41682, 32586],
                    [41759, 32585],
                    [41758, 32617],
                    [41863, 32615],
                    [41864, 32383],
                ],
            ),
        ],
    ),
    # ----------------------------- Sorrow's Embrace ----------------------------- #
    MapPatch(
        map_id=DungeonMap.SE_STORY,
        default=Buffs(utility=DREDGE_SLAYING, sigil_night=NIGHT_SIGIL, sigil_slaying=DREDGE_SIGIL),
        sector_patches=[
            NewSector(
                sector_id=-1,
                name="Inspector Snik",
                buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
                bounds=[
                    [52613, 34438],
                    [52611, 34544],
                    [52486, 34539],
                    [52438, 34430],
                ],
            ),
            NewSector(
                sector_id=-2,
                name="Advanced Assault Golem",
                buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
                bounds=[
                    [52609, 34433],
                    [52608, 34565],
                    [52869, 34565],
                    [52868, 34439],
                ],
            ),
            NewSector(
                sector_id=-3,
                name="Inquest Base",
                buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
                bounds=[
                    [53213, 34426],
                    [53216, 34651],
                    [53123, 34691],
                    [53271, 35086],
                    [53618, 34731],
                    [53605, 34405],
                ],
            ),
            NewSector(
                sector_id=-4,
                name="Forgeman",
                buffs=Buffs(utility=NO_SLAYING_POTION, sigil_slaying=NO_SLAYING_SIGIL),
                bounds=[
                    [52958, 34911],
                    [53298, 34870],
                    [53299, 35310],
                    [52921, 35310],
                ],
            ),
        ],
    ),
    MapPatch(
        map_id=DungeonMap.SE_EXPLORABLE,
        default=Buffs(utility=DREDGE_SLAYING, sigil_night=NIGHT_SIGIL, sigil_slaying=DREDGE_SIGIL),
        sector_patches=[
            # p1
            SectorPatch(
                sector_id=174, buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL)
            ),
            SectorPatch(
                sector_id=179, buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL)
            ),
            SectorPatch(
                sector_id=180, buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL)
            ),
            SectorPatch(
                sector_id=181, buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL)
            ),
            # p3 cart
            SectorPatch(
                sector_id=172,
                buffs=Buffs(utility=NO_SLAYING_POTION, sigil_slaying=NO_SLAYING_SIGIL),
            ),
            # p3 endboss
            SectorPatch(
                sector_id=173, buffs=Buffs(utility=DESTROYER_SLAYING, sigil_slaying=DESTORYER_SIGIL)
            ),
        ],
    ),
    # ----------------------------- Citadel of Flame ----------------------------- #
    MapPatch(
        map_id=DungeonMap.CoF_STORY,
        default=Buffs(
            utility=FLAME_LEGION_SLAYING, sigil_night=NIGHT_SIGIL, sigil_slaying=FLAME_LEGION_SIGIL
        ),
        sector_patches=[],
    ),
    MapPatch(
        map_id=DungeonMap.CoF_EXPLORABLE,
        default=Buffs(
            utility=FLAME_LEGION_SLAYING, sigil_night=NIGHT_SIGIL, sigil_slaying=FLAME_LEGION_SIGIL
        ),
        sector_patches=[],
    ),
    # ---------------------------- Honor of the Waves ---------------------------- #
    MapPatch(
        map_id=DungeonMap.HotW_STORY,
        default=Buffs(
            utility=ICEBROOD_SLAYING,
            sigil_night=NO_NIGHT_SIGIL,
            sigil_slaying=ICEBROOD_SIGIL,
        ),
        sector_patches=[
            # Kulag the Fallen
            SectorPatch(
                sector_id=661,
                buffs=Buffs(utility=SONS_OF_SVANIR_SLAYING, sigil_slaying=SONS_OF_SVANIR_SIGIL),
            ),
            # Lani Winterfist
            SectorPatch(
                sector_id=669,
                buffs=Buffs(utility=SONS_OF_SVANIR_SLAYING, sigil_slaying=SONS_OF_SVANIR_SIGIL),
            ),
            # Lani Winterfist (can move a lot)
            SectorPatch(
                sector_id=667,
                buffs=Buffs(utility=SONS_OF_SVANIR_SLAYING, sigil_slaying=SONS_OF_SVANIR_SIGIL),
            ),
        ],
    ),
    MapPatch(
        map_id=DungeonMap.HotW_EXPLORABLE,
        default=Buffs(
            utility=ICEBROOD_SLAYING, sigil_night=NO_NIGHT_SIGIL, sigil_slaying=ICEBROOD_SIGIL
        ),
        sector_patches=[
            # Aldus Stormbringer (p1)
            SectorPatch(
                sector_id=658,
                buffs=Buffs(utility=SONS_OF_SVANIR_SLAYING, sigil_slaying=SONS_OF_SVANIR_SIGIL),
            ),
            # Andal The Thug (p2)
            NewSector(
                sector_id=-1,
                name="Andal the Thug",
                buffs=Buffs(utility=SONS_OF_SVANIR_SLAYING, sigil_slaying=SONS_OF_SVANIR_SIGIL),
                floors=[6, 7],  # standing on the closed hole in the ground puts you on floor 6
                bounds=[
                    [55235, 25073],
                    [55174, 25073],
                    [55175, 24949],
                    [55236, 24950],
                ],
            ),
            # Fimbul (p3)
            SectorPatch(
                sector_id=669,
                buffs=Buffs(utility=SONS_OF_SVANIR_SLAYING, sigil_slaying=SONS_OF_SVANIR_SIGIL),
            ),
        ],
    ),
    # --------------------------- Crucible of Eternity --------------------------- #
    MapPatch(
        map_id=DungeonMap.CoE_STORY,
        default=Buffs(
            utility=INQUEST_SLAYING, sigil_night=NO_NIGHT_SIGIL, sigil_slaying=INQUEST_SIGIL
        ),
        floors=[-11, -12],
        sector_patches=[
            NewSector(
                sector_id=-1,
                name="Kudu",
                floors=[-12],  # custom floor
                buffs=Buffs(utility=NO_SLAYING_POTION, sigil_slaying=NO_SLAYING_SIGIL),
                bounds=[
                    [53860, 38652],
                    [53639, 38677],
                    [53569, 38671],
                    [53321, 38582],
                    [53365, 38363],
                    [53702, 38141],
                    [53857, 38148],
                    [53859, 38330],
                    [53695, 38334],
                    [53456, 38548],
                    [53671, 38571],
                    [53860, 38573],
                ],
            )
        ],
    ),
    MapPatch(
        map_id=DungeonMap.CoE_EXPLORABLE,
        default=Buffs(
            utility=UNDEAD_SLAYING, sigil_night=NO_NIGHT_SIGIL, sigil_slaying=UNDEAD_SIGIL
        ),
        floors=[-10, -11],
        sector_patches=[
            # The Aquarium
            SectorPatch(
                sector_id=407,
                buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
            ),
            # Arcanic Reactor
            SectorPatch(
                sector_id=405,
                buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
            ),
            # Path to Golem
            NewSector(
                sector_id=-1,
                name="Path to Golem",
                buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
                bounds=[
                    [53701, 38585],
                    [53711, 38334],
                    [53827, 38329],
                    [53846, 38338],
                    [53846, 38348],
                    [53859, 38362],
                    [53893, 38366],
                    [53856, 38405],
                    [53795, 38391],
                    [53793, 38585],
                ],
            ),
            # Golem
            SectorPatch(
                sector_id=400,
                buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
            ),
            # Console Room
            SectorPatch(
                sector_id=401,
                buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
            ),
            # Bjarl the Rampager
            SectorPatch(
                sector_id=403,
                buffs=Buffs(utility=ICEBROOD_SLAYING, sigil_slaying=ICEBROOD_SIGIL),
            ),
            # Cannon
            SectorPatch(
                sector_id=399,
                buffs=Buffs(utility=INQUEST_SLAYING, sigil_slaying=INQUEST_SIGIL),
            ),
            NewSector(
                sector_id=-2,
                name="Husk",
                buffs=Buffs(utility=NO_SLAYING_POTION, sigil_slaying=NO_SLAYING_SIGIL),
                bounds=[
                    [53623, 37703],
                    [53820, 37704],
                    [53824, 37816],
                    [53613, 37805],
                ],
            ),
            # Destroyer path
            SectorPatch(
                sector_id=392,
                buffs=Buffs(utility=DESTROYER_SLAYING, sigil_slaying=DESTORYER_SIGIL),
            ),
            SectorPatch(
                sector_id=395,
                buffs=Buffs(utility=DESTROYER_SLAYING, sigil_slaying=DESTORYER_SIGIL),
            ),
            NewSector(
                sector_id=-3,
                name="Thank you Anet for that gap",
                buffs=Buffs(utility=DESTROYER_SLAYING, sigil_slaying=DESTORYER_SIGIL),
                bounds=[
                    [53610, 37939],
                    [53633, 37940],
                    [53633, 37966],
                    [53610, 37967],
                ],
            ),
            # Destroyer Boss
            SectorPatch(
                sector_id=394,
                buffs=Buffs(utility=DESTROYER_SLAYING, sigil_slaying=DESTORYER_SIGIL),
            ),
            # Front Door
            NewSector(
                sector_id=-4,
                name="Front Door",
                floors=[-10],  # custom floor
                buffs=Buffs(utility=UNDEAD_SLAYING, sigil_slaying=UNDEAD_SIGIL),
                bounds=[
                    [53859, 38306],
                    [53846, 38337],
                    [53846, 38347],
                    [53859, 38360],
                    [53882, 38363],
                    [53913, 38330],
                ],
            ),
        ],
    ),
    # ----------------------------------- Arah ----------------------------------- #
    MapPatch(
        map_id=DungeonMap.ARAH_EXPLORABLE,
        default=Buffs(
            utility=UNDEAD_SLAYING, sigil_night=NO_NIGHT_SIGIL, sigil_slaying=UNDEAD_SIGIL
        ),
        sector_patches=[
            # P1 Shoggroth
            NewSector(
                sector_id=-1,
                name="Shoggroth",
                buffs=Buffs(utility=ELEMENTAL_SLAYING, sigil_slaying=ELEMENTAL_SIGIL),
                bounds=[
                    [45418, 43636],
                    [45575, 43664],
                    [45694, 43666],
                    [45681, 43572],
                    [45589, 43497],
                ],
            ),
            # P1 Jotun
            NewSector(
                sector_id=-2,
                name="Jotun",
                buffs=Buffs(utility=GHOST_SLAYING, sigil_slaying=GHOST_SIGIL),
                bounds=[
                    [45809, 44685],
                    [45807, 44919],
                    [45350, 44921],
                    [45405, 44675],
                ],
            ),
        ],
    ),
]
