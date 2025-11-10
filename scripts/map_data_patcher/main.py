import json
from copy import deepcopy
from dataclasses import asdict, is_dataclass
from pathlib import Path

from buffs import (
    DESTROYER_SLAYING,
    DREDGE_SLAYING,
    FLAME_LEGION_SLAYING,
    GHOST_SLAYING,
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
    # ----------------------------- NIGHTMARE FRACTAL ---------------------------- #
    MapPatch(
        map_id=FractalMap.NIGHTMARE,
        default=Buffs(utility=SCARLETS_ARMIES_SLAYING),
        sector_patches=[
            SectorPatch(
                sector_id=1412,
                name="Spawn",
                buffs=Buffs(utility=SONS_OF_SVANIR_SLAYING),
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
                buffs=Buffs(utility=GHOST_SLAYING),
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
                buffs=Buffs(utility=GHOST_SLAYING),
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
                buffs=Buffs(sigil=SERPENT_SIGIL),
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
                buffs=Buffs(utility=SHARPENING_STONE),
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
                buffs=Buffs(utility=SERPENT_SIGIL),
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
                buffs=Buffs(utility=NIGHTMARE_COURT_SLAYING),
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
                buffs=Buffs(utility=SCARLETS_ARMIES_SLAYING),
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
        default=Buffs(utility=SHARPENING_STONE),
        sector_patches=[
            SectorPatch(
                sector_id=1840,
                buffs=Buffs(utility=ICEBROOD_SLAYING),
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
                buffs=Buffs(sigil=SERPENT_SIGIL),
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

        for sector_patch in patch.sector_patches:
            sector = map_data["sectors"].get(str(sector_patch.sector_id))

            if isinstance(sector_patch, NewSector):
                if sector:
                    print(f"Sector {sector_patch.sector_id} already exists on map {patch.map_id}")
                    continue

                map_data["sectors"][str(sector_patch.sector_id)] = {
                    "id": sector_patch.sector_id,
                    "name": sector_patch.name,
                    "bounds": sector_patch.bounds,
                    "buffs": sector_patch.buffs,
                }

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
