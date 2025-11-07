# Prep Alert

...

## Features

...

## Development

### Map Data

Data is fetched manually on demand from the GW2 Wiki using `scripts/get-maps.py`.  
The widget [Interactive map data builder](https://wiki.guildwars2.com/wiki/Widget:Interactive_map_data_builder) (more specifically the [locator-map-output.js](https://wiki.guildwars2.com/wiki/Widget:Interactive_map_data_builder/locator-map-output.js) for sector data) provides a usable source of map information. The official API endpoint contains several inconsistencies that make it difficult to use directly.

The retrieved map data is stored in `data/maps_raw.json`.

Afterward, the data is processed with `scripts/map_data_patcher/main.py` to generate `src/maps.json`.


### Reverse Engineering

...

## Credits

- Raidcore's Nexus addon platform and community
- Gw2 wiki for map data
