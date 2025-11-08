# Prep Alert

A Guild Wars 2 Nexus addon that alerts the player when important buffs such as enhancements and sigils are missing.  
These alerts show up dynamically based on the player's current position.

If a buff is activated, the alert will disappear automatically.

<img src="./docs/preview.png?" alt="Preview of Prep Alert in action" />

## Features

For all supported maps, more specifically for boss encounters, there are utility (enhancements) and/or sigil reminders.

### Supported maps

- All 8 Dungeons (Story and Explorable)
  - Ascalonian Catacombs
  - Caudecus's Manor
  - Twilight Arbor
  - Sorrow's Embrace
  - Citadel of Flame
  - Honor of the Waves
  - Crucible of Eternity
  - Arah

- All Challenge Mode Fractals
  - Lonely Tower
  - Silent Surf
  - Sunqua Peak
  - Shattered Observatory
  - Nightmare
  - Kinfall

## Technical Details

The addon itself is written in C++ 20 for the Nexus addon host.  
The scripts for map data fetching and processing are written in Python 3.12+.

### Map Data

Data is fetched manually on demand from the GW2 Wiki using `scripts/get-maps.py`.  
The widget [Interactive map data builder](https://wiki.guildwars2.com/wiki/Widget:Interactive_map_data_builder) (more specifically the [locator-map-output.js](https://wiki.guildwars2.com/wiki/Widget:Interactive_map_data_builder/locator-map-output.js) for sector data) provides a reliable source of map information. The official API endpoint contains several inconsistencies that make it difficult to use directly.

The retrieved map data is stored in `data/maps_raw.json`.

Afterward, the data is processed with `scripts/map_data_patcher/main.py` to generate `src/maps.json` which is then packed into the addon DLL.

### Memory Reading

A tiny portion of the addon relies on memory reading to retrieve currently active buffs on the player.  
Should be safe but as always, use at your own risk.

### Dependencies

- [Nexus](https://raidcore.gg/Nexus)
- [ImGui](https://github.com/ocornut/imgui)
- [nlohmann/json](https://github.com/nlohmann/json)

## Credits

- Raidcore's Nexus addon platform and community (especially Delta)
- The dependencies used
- Gw2 wiki for a usable map data API
