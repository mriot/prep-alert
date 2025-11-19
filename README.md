# Prep Alert

A Guild Wars 2 Nexus addon that alerts the player when important buffs such as
<sub><img src="./docs/utility.png" height="20" /></sub> Enhancements,
<sub><img src="./docs/sigil.png" height="20" /></sub> Sigils or
<sub><img src="./docs/food.png" height="20" /></sub> Food
are missing.

<img src="./docs/preview.png?" alt="Preview of Prep Alert in action" />

## Features

Reminders for
<sub><img src="./docs/utility.png" height="20" /></sub> Enhancements,
<sub><img src="./docs/sigil.png" height="20" /></sub> Sigils and
<sub><img src="./docs/food.png" height="20" /></sub> Food show up dynamically based on the player's current position in
supported maps.  
The reminder will only show up when the respective buff is missing.

## Options

...

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

- Fractals (CMs only)
    - Kinfall
    - Nightmare
    - Shattered Observatory
    - Sunqua Peak
    - Silent Surf
    - Lonely Tower

## Technical Details

The addon itself is written in C++ 20 for the Nexus addon host.  
The scripts for map data fetching and processing are written in Python 3.11+.

### Map Data

Data is fetched manually on demand from the GW2 Wiki using `scripts/get-maps.py`.  
The widget [Interactive map data builder](https://wiki.guildwars2.com/wiki/Widget:Interactive_map_data_builder) (more
specifically
the [locator-map-output.js](https://wiki.guildwars2.com/wiki/Widget:Interactive_map_data_builder/locator-map-output.js)
for sector data) provides a reliable source of map information.  
The official API endpoint contains several inconsistencies that make it difficult to use directly.

The retrieved map data is stored in `data/maps_raw.json`.

Afterward, the data is processed with `scripts/map_data_patcher/main.py` to generate `src/maps.json` which is then
packed into the addon DLL.

### Memory Reading

A small portion of the addon relies on memory reading to

- retrieve currently active buffs on the player
- get the current map floor the player is on

Should be safe but as always, use at your own risk.

### Dependencies

- [Nexus](https://raidcore.gg/Nexus)
- [ImGui](https://github.com/ocornut/imgui)
- [nlohmann/json](https://github.com/nlohmann/json)

## Credits

- Raidcore's Nexus addon platform and community (especially Delta)
- The Gw2 Wiki, for a usable map data API
- Elis and Baste, for support while mapping fractals
- The `[TY]` guild, for all the fun we have in dungeons
