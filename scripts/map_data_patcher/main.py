import json
from copy import deepcopy
from dataclasses import asdict, is_dataclass
from enum import Enum
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
        floors=[63, 64],  # TODO not currently patched into map data (not really needed either)
        sector_patches=[
            SectorPatch(
                name="Lower floor",  # default floor 63
                sector_id=2016,
                buffs=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
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
                buffs=Buffs(utility=DEMON_SLAYING, sigil=DEMON_SIGIL),
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
        default=Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
        sector_patches=[
            SectorPatch(
                sector_id=1478,
                buffs=Buffs(utility=SCARLETS_ARMIES_SLAYING),
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
        default=Buffs(utility=SCARLETS_ARMIES_SLAYING, sigil=SERPENT_SIGIL),
        sector_patches=[
            SectorPatch(
                sector_id=1412,
                name="Spawn + MAMA",
                buffs=Buffs(sigil=GENERIC_SIGIL),
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
        default=Buffs(sigil=NIGHT_SIGIL, utility=GENERIC_ENHANCEMENT),
        sector_patches=[],
    ),
    # ---------------------------------------------------------------------------- #
    #                                   DUNGEONS                                   #
    # ---------------------------------------------------------------------------- #
    # ----------------------------- Ascalon Catacombs ---------------------------- #
    MapPatch(
        DungeonMap.AC_STORY,
        Buffs(utility=GHOST_SLAYING, sigil=NIGHT_SIGIL),
        [],
    ),
    MapPatch(
        DungeonMap.AC_EXPLORABLE,
        Buffs(utility=GENERIC_ENHANCEMENT, sigil=NIGHT_SIGIL),
        [
            SectorPatch(9, Buffs(utility=GHOST_SLAYING)),
        ],
    ),
    # ----------------------------- Caudecus's Manor ----------------------------- #
    MapPatch(
        DungeonMap.CM_STORY,
        Buffs(utility=OUTLAW_SLAYING, sigil=GENERIC_SIGIL),
        [],
    ),
    MapPatch(
        DungeonMap.CM_EXPLORABLE,
        Buffs(utility=OUTLAW_SLAYING, sigil=GENERIC_SIGIL),
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
        Buffs(utility=SONS_OF_SVANIR_SLAYING, sigil=GENERIC_SIGIL),
        [
            # Honor's Voice
            SectorPatch(660, Buffs(utility=ICEBROOD_SLAYING)),
            # Kodan's Bane
            SectorPatch(659, Buffs(utility=ICEBROOD_SLAYING)),
        ],
    ),
    MapPatch(
        DungeonMap.HotW_EXPLORABLE,
        Buffs(utility=ICEBROOD_SLAYING, sigil=GENERIC_SIGIL),
        [
            # Aldus Stormbringer (p1)
            SectorPatch(658, Buffs(utility=SONS_OF_SVANIR_SLAYING))
        ],
    ),
    # --------------------------- Crucible of Eternity --------------------------- #
    MapPatch(
        DungeonMap.CoE_STORY,
        Buffs(utility=INQUEST_SLAYING, sigil=GENERIC_SIGIL),
        floors=[-11, -12],  # TODO not currently patched into map data (not really needed either)
        sector_patches=[
            NewSector(
                sector_id=2001,
                name="Kudu",
                floors=[-12],  # custom floor
                buffs=Buffs(utility=GENERIC_ENHANCEMENT),
                bounds=[
                    [53699, 38218],
                    [53695, 38324],
                    [53822, 38324],
                    [53818, 38215],
                ],
            )
        ],
    ),
    MapPatch(
        DungeonMap.CoE_EXPLORABLE,
        Buffs(utility=GENERIC_ENHANCEMENT, sigil=GENERIC_SIGIL),
        [],
    ),
    # ----------------------------------- Arah ----------------------------------- #
    MapPatch(
        DungeonMap.ARAH_EXPLORABLE,
        Buffs(utility=UNDEAD_SLAYING, sigil=GENERIC_SIGIL),
        [
            # TODO P1 Shoggroth
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
                map_data["continent_id"] = int(continent_id)
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
#                              CUSTOM JSON ENCODER                             #
# ---------------------------------------------------------------------------- #
class CustomJSONEncoder(json.JSONEncoder):
    def default(self, o):
        # Dataclasses
        if is_dataclass(o) and not isinstance(o, type):
            result = asdict(o)
            for k, v in result.items():
                # Enums inside dataclasses
                if isinstance(v, Enum):
                    result[k] = v.value
            return result
        # Enums
        if isinstance(o, Enum):
            return o.value
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
    maps = strip_unpatched_sectors(maps)
    maps = convert_sectors_to_list(maps)

    with open(dest_file, "w", encoding="utf-8") as f:
        json.dump(
            maps,
            f,
            indent=None,
            separators=(",", ":"),
            ensure_ascii=False,
            cls=CustomJSONEncoder,
        )

    print(f"Patched map data written to {dest_file}")
