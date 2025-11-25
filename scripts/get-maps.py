# /// script
# requires-python = ">=3.12"
# dependencies = [
#     "requests",
# ]
# ///
import json
import re
from pathlib import Path

import requests

url = "https://wiki.guildwars2.com/index.php?title=Widget:Interactive_map_data_builder/locator-map-output.js&action=raw&ctype=text/javascript"

r = requests.get(url, timeout=10)
if r.status_code != 200:
    raise RuntimeError(f"HTTP {r.status_code} while fetching data")

match = re.search(r"=\s*(\{.*\});?", r.text, re.DOTALL)
if not match:
    raise ValueError("No JSON found in response")

try:
    with open(Path(__file__).parents[1] / "data" / "maps_raw.json", "w", encoding="utf-8") as f:
        json.dump(json.loads(match.group(1)), f, indent=4, ensure_ascii=False)
except json.JSONDecodeError as e:
    raise ValueError(f"Invalid JSON: {e}")
