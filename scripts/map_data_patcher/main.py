import argparse
import json
import sys
from copy import deepcopy
from dataclasses import asdict, is_dataclass
from enum import Enum
from pathlib import Path

from models import DungeonMap, FractalMap, NewSector, SectorPatch
from patches import PATCHES


# ---------------------------------------------------------------------------- #
#                                  MAP LOADER                                  #
# ---------------------------------------------------------------------------- #
def load_raw_maps(file_path: Path, map_ids: list[int]) -> dict:
    """Load and filter map data from raw JSON file."""
    with open(file_path, encoding="utf-8") as f:
        data = json.load(f)

    maps = {}

    for continent_id, continent in data.items():
        for map_id, map_data in continent["maps"].items():
            if int(map_id) in map_ids:
                map_data["continent_id"] = int(continent_id)
                # map_data["continent_name"] = continent["name"]
                # map_data["continent_dims"] = continent["continent_dims"]

                maps[int(map_id)] = map_data

    return maps


# ---------------------------------------------------------------------------- #
#                             STORY DUPLICATE MAPS                             #
# ---------------------------------------------------------------------------- #
def derive_dungeon_story_maps(maps: dict) -> dict:
    """Create story mode map variants by cloning explorable dungeon maps."""
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
    """Apply sector patches and buffs to map data."""
    for patch in patches:
        map_data = dungeon_maps[patch.map_id]

        map_data.setdefault("default_buffs", patch.default)

        # usually only set when new floors are added
        if patch.floors is not None:
            map_data["floors"] = patch.floors

        for sector_patch in patch.sector_patches:
            sector = map_data["sectors"].get(str(sector_patch.sector_id))

            # NEW SECTORS
            if isinstance(sector_patch, NewSector):
                if sector:
                    print(f"Sector {sector_patch.sector_id} already exists on map {patch.map_id}")
                    sys.exit(1)
                if sector_patch.sector_id >= 0:
                    print(
                        f"New sector {sector_patch.name} on map {patch.map_id} must use a negative ID"
                    )
                    sys.exit(1)

                map_data["sectors"][str(sector_patch.sector_id)] = {
                    "id": sector_patch.sector_id,
                    "name": sector_patch.name,
                    "bounds": sector_patch.bounds,
                    "buffs": sector_patch.buffs,
                    "floors": sector_patch.floors or [map_data.get("default_floor")],
                }

            # EXISTING SECTORS
            if isinstance(sector_patch, SectorPatch):
                if not sector:
                    print(f"Sector {sector_patch.sector_id} not found on map {patch.map_id}")
                    sys.exit(1)

                sector["buffs"] = sector_patch.buffs

                if sector_patch.name:
                    sector["name"] = sector_patch.name

    return dungeon_maps


# ---------------------------------------------------------------------------- #
#                            STRIP UNPATCHED SECTORS                           #
# ---------------------------------------------------------------------------- #
def strip_unpatched_sectors(maps: dict) -> dict:
    """Remove sectors that don't have buff data."""
    for map_id, map_data in maps.items():
        for sector_id, sector_data in list(map_data["sectors"].items()):
            if "buffs" not in sector_data:
                del map_data["sectors"][sector_id]
    return maps


# ---------------------------------------------------------------------------- #
#                                SECTORS TO LIST                               #
# ---------------------------------------------------------------------------- #
def convert_sectors_to_list(maps: dict) -> dict:
    """Convert sector dictionaries to sorted lists."""
    result = {}

    for map_id, map_data in maps.items():
        new_map = deepcopy(map_data)

        if isinstance(new_map.get("sectors"), dict):
            new_map["sectors"] = sorted(list(new_map["sectors"].values()), key=lambda s: s["id"])

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
    arg_pargser = argparse.ArgumentParser()
    arg_pargser.add_argument("--debug", action="store_true")
    args = arg_pargser.parse_args()

    root = Path(__file__).parents[2]

    src_file = root / "data" / "maps_raw.json"
    dest_file = root / "src" / "maps.json"

    map_ids = [m.value for m in DungeonMap] + [f.value for f in FractalMap]

    maps = load_raw_maps(src_file, map_ids)
    maps = derive_dungeon_story_maps(maps)
    maps = apply_patches(PATCHES, maps)
    if not args.debug:
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

    file_size = dest_file.stat().st_size // 1024
    print(f"Patched map data written to {dest_file} ({file_size} KB)")
