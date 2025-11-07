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
    # MapPatch(
    #     map_id=FractalMap.NIGHTMARE,
    #     default=Buffs(utility=SCARLETS_ARMIES_SLAYING),
    #     sector_patches=[
    #         SectorPatch(
    #             sector_id=1412,
    #             buffs=Buffs(utility=SCARLETS_ARMIES_SLAYING),
    #         ),
    #     ],
    # ),
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
