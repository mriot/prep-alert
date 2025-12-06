# Prep Alert

A Guild Wars 2 Nexus addon that reminds you when important buffs such as
<sub><img src="./docs/utility.png" height="20" /></sub> Enhancements and
<sub><img src="./docs/sigil.png" height="20" /></sub> Sigils are missing.


<img src="./docs/demo.png" alt="Buff reminders demo" height="350" />


## Features

In **Dungeons** and **Fractal CMs** these reminders show up dynamically based on your current position.  
They will only show up when the respective buff is missing.

You can also pick which buffs to show, e.g. if you don't care about Slaying Sigils you can disable those reminders.

If no special buffs are available for the current encounter, an alert reminds you to use your build's default buffs.

<img src="./docs/default_buff.png?" alt="Default buff reminder" align="" />

### Buff Choices

The buff choices are based on the [Instanced Species List](https://docs.google.com/spreadsheets/d/135Iac7sWfB55kn3kFXWULneu4VfqmK8JqCHCrx3_ZMo/) and focus on general usefulness rather than going for the _absolute_ tryhard setup.  
Let me know if you think something should be changed.

You can check it out using the [interactive mapping tool](https://riotcoding.com/gw2/prepalert/mapping/).

> [!NOTE]
> Currently, custom floors for some maps are not functional in the mapping tool but work just fine in-game.

## Options

Hovering the `(?)` in-game will show additional information.

<img src="./docs/options.png?" alt="Prep Alert Options" />

## Technical Stuff

The addon is written in C++ 20 for the [Nexus addon platform](https://raidcore.gg/Nexus).  
The scripts for map data fetching and processing are written in Python 3.11.

### Memory Reading ⚠️

A small portion of the addon relies on memory reading to

- retrieve currently active buffs on the player
- get the current map floor the player is on

#### Should be safe but as always, use at your own risk

> [!NOTE]
> In order to keep the addon open source, game client internals were moved to a separate closed-source dependency.  

### Map Data

Map data, originally fetched from the GW2 Wiki, is processed by [the map data patcher](./scripts/map_data_patcher/main.py) to produce `src/maps.json`. During this step, the patcher updates sector definitions by adding buff information, or creating additional sectors where necessary.

Custom sectors were created using the [interactive mapping tool](https://riotcoding.com/gw2/prepalert/mapping/).

### Dependencies

- [Nexus](https://raidcore.gg/Nexus)
- [ImGui](https://github.com/ocornut/imgui)
- [nlohmann/json](https://github.com/nlohmann/json)

## Credits

- Raidcore's Nexus addon platform and community (especially Delta)
- The Gw2 Wiki, for a usable map data API and map widgets
- The folks who created the [Instanced Species List](https://docs.google.com/spreadsheets/d/135Iac7sWfB55kn3kFXWULneu4VfqmK8JqCHCrx3_ZMo/)
- Elis, Baste and especially Bird for support and testing
- The `[TY]` guild, for all the fun dungeon runs
