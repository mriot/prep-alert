import json
from copy import deepcopy
from dataclasses import asdict, is_dataclass
from pathlib import Path

from buffs import (
    DEMON_SIGIL,
    DEMON_SLAYING,
    DESTROYER_SLAYING,
    DREDGE_SLAYING,
    FLAME_LEGION_SLAYING,
    GENERIC_ENHANCEMENT,
    GENERIC_SIGIL,
    GHOST_SLAYING,
    ICEBROOD_SIGIL,
    ICEBROOD_SLAYING,
    INQUEST_SLAYING,
    NIGHT_SIGIL,
    NIGHTMARE_COURT_SLAYING,
    OUTLAW_SLAYING,
    SCARLETS_ARMIES_SLAYING,
    SERPENT_SIGIL,
    SHARPENING_STONE,
    SONS_OF_SVANIR_SLAYING,
    UNDEAD_SLAYING,
)
from models import Buffs, DungeonMap, FractalMap, MapPatch, NewSector, SectorPatch

# ---------------------------------------------------------------------------- #
#                                    PATCHES                                   #
# ---------------------------------------------------------------------------- #
PATCHES = [
    # --------------------------- LONELY TOWER FRACTAL --------------------------- #
    MapPatch(
        map_id=FractalMap.LONELY_TOWER,
        default=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
        floors=[63, 64],  # TODO not currently patched into map data
        sector_patches=[
            SectorPatch(
                name="Lower floor",  # is on default floor 63
                sector_id=2016,
                buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
                bounds=[
                    [0, 0],  # TODO
                ],
            ),
            NewSector(
                sector_id=2017,
                name="Upper floor",
                buffs=Buffs(utility=DEMON_SLAYING, sigil=DEMON_SIGIL),
                floors=[64],  # this is a custom floor and handled in the addon
                bounds=[
                    [0, 0],  # TODO
                ],
            ),
        ],
    ),
    # --------------------------- SHATTERED OBSERVATORY -------------------------- #
    MapPatch(
        map_id=FractalMap.SHATTERED_OBSERVATORY,
        default=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
        sector_patches=[
            SectorPatch(
                sector_id=1478,
                buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            ),
            # disable default sector when enabling new sectors
            # NewSector(
            #     sector_id=1478,
            #     name="Spawn",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [11292, 4683],
            #         [11300, 4723],
            #         [11340, 4728],
            #         [11367, 4727],
            #         [11368, 4720],
            #         [11361, 4716],
            #         [11323, 4705],
            #         [11315, 4690],
            #         [11306, 4675],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1479,
            #     name="Skorvald",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [11456, 4698],
            #         [11441, 4664],
            #         [11417, 4654],
            #         [11382, 4661],
            #         [11367, 4689],
            #         [11369, 4712],
            #         [11379, 4730],
            #         [11391, 4741],
            #         [11403, 4747],
            #         [11432, 4743],
            #         [11453, 4723],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1480,
            #     name="Platform 1",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [11358, 4753],
            #         [11341, 4742],
            #         [11325, 4760],
            #         [11325, 4779],
            #         [11332, 4795],
            #         [11347, 4792],
            #         [11357, 4784],
            #         [11364, 4775],
            #         [11362, 4759],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1481,
            #     name="Platform 2",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [11497, 4766],
            #         [11495, 4751],
            #         [11476, 4745],
            #         [11459, 4761],
            #         [11456, 4779],
            #         [11460, 4794],
            #         [11469, 4793],
            #         [11487, 4781],
            #         [11495, 4770],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1482,
            #     name="Platform 3",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [11477, 4617],
            #         [11467, 4624],
            #         [11465, 4634],
            #         [11473, 4646],
            #         [11492, 4659],
            #         [11511, 4641],
            #         [11509, 4626],
            #         [11501, 4617],
            #         [11491, 4617],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1483,
            #     name="Platform 4",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [11316, 4642],
            #         [11329, 4652],
            #         [11347, 4654],
            #         [11361, 4650],
            #         [11362, 4630],
            #         [11355, 4611],
            #         [11340, 4613],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1484,
            #     name="Portal HUB",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [12899, 5054],
            #         [12854, 5007],
            #         [12682, 4995],
            #         [12669, 4995],
            #         [12614, 5034],
            #         [12614, 5074],
            #         [12669, 5113],
            #         [12681, 5113],
            #         [12855, 5101],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1485,
            #     name="Golem Island 1",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [13003, 5218],
            #         [12962, 5205],
            #         [12910, 5218],
            #         [12921, 5233],
            #         [12932, 5254],
            #         [13004, 5228],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1486,
            #     name="Golem Island 2 (Ball)",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [12862, 5200],
            #         [12832, 5190],
            #         [12803, 5193],
            #         [12786, 5242],
            #         [12785, 5277],
            #         [12815, 5276],
            #         [12862, 5250],
            #         [12877, 5235],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1487,
            #     name="Golem Island 3",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [12893, 5348],
            #         [12933, 5343],
            #         [12957, 5367],
            #         [12995, 5406],
            #         [12974, 5444],
            #         [12941, 5463],
            #         [12914, 5446],
            #         [12892, 5416],
            #         [12880, 5395],
            #         [12880, 5370],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1488,
            #     name="Pre Artsariiv",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [12828, 5465],
            #         [12843, 5458],
            #         [12850, 5442],
            #         [12842, 5405],
            #         [12830, 5390],
            #         [12812, 5396],
            #         [12798, 5402],
            #         [12782, 5419],
            #         [12772, 5436],
            #         [12772, 5452],
            #         [12814, 5488],
            #         [12828, 5464],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1489,
            #     name="Artsariiv",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [12716, 5433],
            #         [12715, 5320],
            #         [12602, 5320],
            #         [12602, 5433],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1490,
            #     name="Pre Arkk",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [11676, 6138],
            #         [11648, 6133],
            #         [11617, 6133],
            #         [11617, 6153],
            #         [11648, 6153],
            #         [11676, 6149],
            #     ],
            # ),
            # NewSector(
            #     sector_id=1491,
            #     name="Arkk",
            #     buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
            #     bounds=[
            #         [11536, 6087],
            #         [11425, 6086],
            #         [11424, 6198],
            #         [11536, 6198],
            #     ],
            # ),
        ],
    ),
    # ---------------------------- SILENT SURF FRACTAL --------------------------- #
    MapPatch(
        map_id=FractalMap.SILENT_SURF,
        default=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
        sector_patches=[],
    ),
    # ------------------------------ KINFALL FRACTAL ----------------------------- #
    MapPatch(
        map_id=FractalMap.KINFALL,
        default=Buffs(utility=ICEBROOD_SLAYING, sigil=ICEBROOD_SIGIL),
        sector_patches=[],
    ),
    # ----------------------------- NIGHTMARE FRACTAL ---------------------------- #
    MapPatch(
        map_id=FractalMap.NIGHTMARE,
        default=Buffs(utility=SCARLETS_ARMIES_SLAYING),
        sector_patches=[
            SectorPatch(
                sector_id=1412,
                name="Spawn",
                buffs=Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil=GENERIC_SIGIL),
                bounds=[
                    [12040, 4519],
                    [12001, 4508],
                    [11981, 4544],
                    [11989, 4569],
                    [11996, 4591],
                    [12029, 4573],
                    [12018, 4556],
                    [12044, 4538],
                ],
            ),
            NewSector(
                sector_id=1413,
                name="Barriers",
                buffs=Buffs(utility=SCARLETS_ARMIES_SLAYING),
                floors=[16],
                bounds=[
                    [11993, 4597],
                    [11992, 4622],
                    [12006, 4644],
                    [12041, 4642],
                    [12081, 4619],
                    [12091, 4586],
                    [12111, 4559],
                    [12144, 4542],
                    [12133, 4514],
                    [12104, 4536],
                    [12074, 4570],
                    [12044, 4570],
                    [12013, 4585],
                ],
            ),
            NewSector(
                sector_id=1414,
                name="MAMA",
                buffs=Buffs(utility=SCARLETS_ARMIES_SLAYING),
                floors=[16],
                bounds=[
                    [12146, 4542],
                    [12186, 4584],
                    [12212, 4597],
                    [12247, 4593],
                    [12269, 4565],
                    [12295, 4522],
                    [12257, 4505],
                    [12210, 4500],
                    [12136, 4516],
                ],
            ),
            NewSector(
                sector_id=1415,
                name="Balls 1",
                buffs=Buffs(sigil=SERPENT_SIGIL, utility=SCARLETS_ARMIES_SLAYING),
                floors=[16],
                bounds=[
                    [12273, 4562],
                    [12306, 4590],
                    [12330, 4608],
                    [12333, 4646],
                    [12313, 4639],
                    [12275, 4650],
                    [12280, 4686],
                    [12306, 4691],
                    [12330, 4669],
                    [12347, 4678],
                    [12362, 4680],
                    [12388, 4686],
                    [12417, 4670],
                    [12410, 4641],
                    [12385, 4636],
                    [12370, 4649],
                    [12349, 4645],
                    [12361, 4590],
                    [12314, 4538],
                ],
            ),
            NewSector(
                sector_id=1416,
                name="Balls 2",
                buffs=Buffs(sigil=SERPENT_SIGIL, utility=SCARLETS_ARMIES_SLAYING),
                floors=[16],
                bounds=[
                    [12347, 4678],
                    [12346, 4702],
                    [12302, 4702],
                    [12299, 4724],
                    [12275, 4725],
                    [12272, 4777],
                    [12286, 4777],
                    [12304, 4787],
                    [12320, 4777],
                    [12335, 4808],
                    [12364, 4812],
                    [12379, 4801],
                    [12382, 4778],
                    [12405, 4786],
                    [12423, 4778],
                    [12429, 4726],
                    [12407, 4725],
                    [12409, 4700],
                    [12361, 4700],
                    [12360, 4683],
                ],
            ),
            NewSector(
                sector_id=1417,
                name="Carolabruecke",
                buffs=Buffs(sigil=SERPENT_SIGIL, utility=SCARLETS_ARMIES_SLAYING),
                floors=[16],
                bounds=[
                    [12177, 4971],
                    [12216, 4973],
                    [12272, 4960],
                    [12319, 4929],
                    [12375, 4877],
                    [12391, 4847],
                    [12379, 4802],
                    [12367, 4815],
                    [12372, 4843],
                    [12354, 4872],
                    [12301, 4923],
                    [12250, 4951],
                    [12206, 4958],
                    [12177, 4957],
                ],
            ),
            NewSector(
                sector_id=1418,
                name="Siax",
                buffs=Buffs(sigil=SERPENT_SIGIL, utility=SCARLETS_ARMIES_SLAYING),
                floors=[16],
                bounds=[
                    [12176, 4956],
                    [12168, 4953],
                    [12174, 4899],
                    [12167, 4888],
                    [12136, 4862],
                    [12135, 4842],
                    [12116, 4846],
                    [12097, 4889],
                    [12089, 4932],
                    [12095, 4962],
                    [12176, 4971],
                ],
            ),
            NewSector(
                sector_id=1419,
                name="Ensolyss",
                buffs=Buffs(sigil=SERPENT_SIGIL, utility=SCARLETS_ARMIES_SLAYING),
                floors=[16],
                bounds=[
                    [12124, 4781],
                    [12149, 4774],
                    [12164, 4758],
                    [12170, 4733],
                    [12161, 4712],
                    [12145, 4696],
                    [12123, 4691],
                    [12098, 4701],
                    [12082, 4720],
                    [12079, 4745],
                    [12090, 4765],
                    [12109, 4779],
                ],
            ),
        ],
    ),
    # ---------------------------- SUNQUA PEAK FRACTAL --------------------------- #
    MapPatch(
        map_id=FractalMap.SUNQUA_PEAK,
        default=Buffs(sigil=NIGHT_SIGIL, utility=GENERIC_ENHANCEMENT),
        sector_patches=[
            SectorPatch(
                sector_id=1840,
                buffs=Buffs(sigil=NIGHT_SIGIL),
                name="CM Spawn",
                bounds=[
                    [11982, 4708],
                    [11981, 4725],
                    [11994, 4747],
                    [11990, 4776],
                    [11971, 4790],
                    [11985, 4798],
                    [11993, 4809],
                    [12011, 4808],
                    [12027, 4801],
                    [12031, 4778],
                    [12014, 4775],
                    [12014, 4738],
                    [11997, 4709],
                    [11994, 4705],
                ],
            ),
            NewSector(
                sector_id=1841,
                name="Boss Arena",
                buffs=Buffs(sigil=NIGHT_SIGIL),
                floors=[51],
                bounds=[
                    [11997, 4810],
                    [11974, 4819],
                    [11956, 4836],
                    [11947, 4858],
                    [11947, 4881],
                    [11957, 4902],
                    [11974, 4917],
                    [11998, 4925],
                    [12022, 4922],
                    [12042, 4910],
                    [12057, 4892],
                    [12062, 4870],
                    [12059, 4848],
                    [12046, 4827],
                    [12028, 4813],
                    [12009, 4810],
                ],
            ),
        ],
    ),
    # ---------------------------------------------------------------------------- #
    # ----------------------------- Ascalon Catacombs ---------------------------- #
    MapPatch(
        DungeonMap.AC_STORY,
        Buffs(utility=GHOST_SLAYING, sigil=NIGHT_SIGIL),
        [],
    ),
    MapPatch(
        DungeonMap.AC_EXPLORABLE,
        Buffs(utility=SHARPENING_STONE, sigil=NIGHT_SIGIL),
        [
            SectorPatch(9, Buffs(utility=GHOST_SLAYING)),
        ],
    ),
    # ----------------------------- Caudecus's Manor ----------------------------- #
    MapPatch(
        DungeonMap.CM_STORY,
        Buffs(utility=OUTLAW_SLAYING),
        [],
    ),
    MapPatch(
        DungeonMap.CM_EXPLORABLE,
        Buffs(utility=OUTLAW_SLAYING),
        [],
    ),
    # ------------------------------ Twilight Arbor ------------------------------ #
    MapPatch(
        DungeonMap.TA_STORY,
        Buffs(utility=NIGHTMARE_COURT_SLAYING, sigil=NIGHT_SIGIL),
        [],
    ),
    MapPatch(
        DungeonMap.TA_EXPLORABLE,
        Buffs(utility=NIGHTMARE_COURT_SLAYING, sigil=NIGHT_SIGIL),
        [
            # Aether
            SectorPatch(1129, Buffs(utility=SCARLETS_ARMIES_SLAYING)),
            SectorPatch(1130, Buffs(utility=SCARLETS_ARMIES_SLAYING)),
            SectorPatch(1131, Buffs(utility=SCARLETS_ARMIES_SLAYING)),
            SectorPatch(1139, Buffs(utility=SCARLETS_ARMIES_SLAYING)),
        ],
    ),
    # ----------------------------- Sorrow's Embrace ----------------------------- #
    MapPatch(
        DungeonMap.SE_STORY,
        Buffs(utility=DREDGE_SLAYING, sigil=NIGHT_SIGIL),
        [],
    ),
    MapPatch(
        DungeonMap.SE_EXPLORABLE,
        Buffs(utility=DREDGE_SLAYING, sigil=NIGHT_SIGIL),
        [
            # p1
            SectorPatch(174, Buffs(utility=INQUEST_SLAYING)),
            SectorPatch(179, Buffs(utility=INQUEST_SLAYING)),
            SectorPatch(180, Buffs(utility=INQUEST_SLAYING)),
            SectorPatch(181, Buffs(utility=INQUEST_SLAYING)),
            # p3 endboss
            SectorPatch(173, Buffs(utility=DESTROYER_SLAYING)),
        ],
    ),
    # ----------------------------- Citadel of Flame ----------------------------- #
    MapPatch(
        DungeonMap.CoF_STORY,
        Buffs(utility=FLAME_LEGION_SLAYING, sigil=NIGHT_SIGIL),
        [],
    ),
    MapPatch(
        DungeonMap.CoF_EXPLORABLE,
        Buffs(utility=FLAME_LEGION_SLAYING, sigil=NIGHT_SIGIL),
        [],
    ),
    # ---------------------------- Honor of the Waves ---------------------------- #
    MapPatch(
        DungeonMap.HotW_STORY,
        Buffs(utility=SONS_OF_SVANIR_SLAYING),
        [
            # Honor's Voice
            SectorPatch(660, Buffs(utility=ICEBROOD_SLAYING)),
            # Kodan's Bane
            SectorPatch(659, Buffs(utility=ICEBROOD_SLAYING)),
        ],
    ),
    MapPatch(
        DungeonMap.HotW_EXPLORABLE,
        Buffs(utility=ICEBROOD_SLAYING),
        [
            # Aldus Stormbringer (p1)
            SectorPatch(658, Buffs(utility=SONS_OF_SVANIR_SLAYING))
        ],
    ),
    # --------------------------- Crucible of Eternity --------------------------- #
    MapPatch(
        DungeonMap.CoE_STORY,
        Buffs(utility=SHARPENING_STONE),
        [],
    ),
    MapPatch(
        DungeonMap.CoE_EXPLORABLE,
        Buffs(utility=SHARPENING_STONE),
        [],
    ),
    # ----------------------------------- Arah ----------------------------------- #
    MapPatch(
        DungeonMap.ARAH_EXPLORABLE,
        Buffs(utility=UNDEAD_SLAYING),
        [
            # P1 Shoggroth
            # SectorPatch(0, Buffs(utility=ELEMENTAL_SLAYING)),
        ],
    ),
]


# ---------------------------------------------------------------------------- #
#                                  MAP LOADER                                  #
# ---------------------------------------------------------------------------- #
def load_raw_maps(file_path: Path, map_ids: list[int]) -> dict:
    with open(file_path, encoding="utf-8") as f:
        data = json.load(f)

    maps = {}

    for continent_id, continent in data.items():
        for map_id, map_data in continent["maps"].items():
            if int(map_id) in map_ids:
                # map_data["continent_id"] = int(continent_id)
                # map_data["continent_name"] = continent["name"]
                # map_data["continent_dims"] = continent["continent_dims"]
                del map_data["continent_rect"]

                maps[int(map_id)] = map_data

    return maps


# ---------------------------------------------------------------------------- #
#                             STORY DUPLICATE MAPS                             #
# ---------------------------------------------------------------------------- #
def derive_dungeon_story_maps(maps: dict) -> dict:
    story_map_links = {
        DungeonMap.AC_STORY: DungeonMap.AC_EXPLORABLE,
        DungeonMap.CM_STORY: DungeonMap.CM_EXPLORABLE,
        DungeonMap.TA_STORY: DungeonMap.TA_EXPLORABLE,
        DungeonMap.SE_STORY: DungeonMap.SE_EXPLORABLE,
        DungeonMap.CoF_STORY: DungeonMap.CoF_EXPLORABLE,
        DungeonMap.HotW_STORY: DungeonMap.HotW_EXPLORABLE,
        DungeonMap.CoE_STORY: DungeonMap.CoE_EXPLORABLE,
    }

    updated_maps = deepcopy(maps)

    for story_id, explorable_id in story_map_links.items():
        if story_id not in updated_maps and explorable_id in updated_maps:
            clone = deepcopy(updated_maps[explorable_id])
            clone["id"] = story_id
            clone["name"] = f"{clone['name']} (Story)"
            updated_maps[story_id] = clone

    return updated_maps


# ---------------------------------------------------------------------------- #
#                                    PATCHER                                   #
# ---------------------------------------------------------------------------- #
def apply_patches(patches, dungeon_maps: dict) -> dict:
    for patch in patches:
        map_data = dungeon_maps[patch.map_id]

        map_data.setdefault("default_buffs", patch.default)
        # TODO patch in map floors

        for sector_patch in patch.sector_patches:
            sector = map_data["sectors"].get(str(sector_patch.sector_id))

            # NEW SECTORS
            if isinstance(sector_patch, NewSector):
                if sector:
                    print(f"Sector {sector_patch.sector_id} already exists on map {patch.map_id}")
                    continue

                map_data["sectors"][str(sector_patch.sector_id)] = {
                    "id": sector_patch.sector_id,
                    "name": sector_patch.name,
                    "bounds": sector_patch.bounds,
                    "buffs": sector_patch.buffs,
                    "floors": sector_patch.floors,
                }

            # EXISTING SECTORS
            if isinstance(sector_patch, SectorPatch):
                if not sector:
                    print(f"Sector {sector_patch.sector_id} not found on map {patch.map_id}")
                    continue

                sector["buffs"] = sector_patch.buffs

                if sector_patch.name:
                    sector["name"] = sector_patch.name

                if sector_patch.bounds:
                    sector["bounds"] = sector_patch.bounds

    return dungeon_maps


# ---------------------------------------------------------------------------- #
#                            STRIP UNPATCHED SECTORS                           #
# ---------------------------------------------------------------------------- #
def strip_unpatched_sectors(maps: dict) -> dict:
    for map_id, map_data in maps.items():
        for sector_id, sector_data in list(map_data["sectors"].items()):
            if "buffs" not in sector_data:
                del map_data["sectors"][sector_id]
    return maps


# ---------------------------------------------------------------------------- #
#                                SECTORS TO LIST                               #
# ---------------------------------------------------------------------------- #
def convert_sectors_to_list(maps: dict) -> dict:
    result = {}

    for map_id, map_data in maps.items():
        new_map = deepcopy(map_data)

        if isinstance(new_map.get("sectors"), dict):
            new_map["sectors"] = list(new_map["sectors"].values())

        result[map_id] = new_map

    return result


# ---------------------------------------------------------------------------- #
#                            DATACLASS JSON ENCODER                            #
# ---------------------------------------------------------------------------- #
class DataclassJSONEncoder(json.JSONEncoder):
    def default(self, o):
        if is_dataclass(o) and not isinstance(o, type):
            return asdict(o)
        return super().default(o)


# ---------------------------------------------------------------------------- #
#                                     MAIN                                     #
# ---------------------------------------------------------------------------- #
if __name__ == "__main__":
    root = Path(__file__).parents[2]

    src_file = root / "data" / "maps_raw.json"
    dest_file = root / "src" / "maps.json"

    map_ids = [m.value for m in DungeonMap] + [f.value for f in FractalMap]

    maps = load_raw_maps(src_file, map_ids)
    maps = derive_dungeon_story_maps(maps)
    maps = apply_patches(PATCHES, maps)
    # maps = strip_unpatched_sectors(maps)
    maps = convert_sectors_to_list(maps)

    with open(dest_file, "w", encoding="utf-8") as f:
        json.dump(
            maps,
            f,
            indent=None,
            separators=(",", ":"),
            ensure_ascii=False,
            cls=DataclassJSONEncoder,
        )

    print(f"Patched map data written to {dest_file}")
