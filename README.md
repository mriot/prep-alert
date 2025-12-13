# PrepAlert (beta)

[![latest release downloads](https://img.shields.io/github/downloads/mriot/prep-alert/latest/PrepAlert.dll?displayAssetName=false&label=latest%20release%20downloads&color=blue)](https://github.com/mriot/prep-alert/releases/latest)
[![total downloads](https://img.shields.io/github/downloads/mriot/prep-alert/total?label=total%20downloads&color=red)](https://github.com/mriot/prep-alert/releases)

A Guild Wars 2 Nexus addon that reminds you when important buffs such as
<sub><img src="./docs/utility.png" height="20" /></sub> Enhancements and
<sub><img src="./docs/sigil.png" height="20" /></sub> Sigils are missing.

<img src="./docs/demo.png" alt="Buff reminders demo" height="350" />

## Features

In **Dungeons** and **Fractal CMs**, these reminders show up dynamically based on your current position.  
They will only appear when the respective buff is missing.

Here in the Sorrow's Embrace, the player is reminded to take Inquest slaying buffs upon entering the P1 area.  
<img src="./docs/demo.gif" alt="Buff reminders demo gif" height="350" />

You can also pick which buffs to show; for example, if you don't care about Slaying Sigils, you can disable those.

If no special buffs are available for the current encounter, you’ll be alerted to replace any lingering buffs.

## Buff Choices

The buff choices are based on the [Instanced Species List](https://docs.google.com/spreadsheets/d/135Iac7sWfB55kn3kFXWULneu4VfqmK8JqCHCrx3_ZMo/) and focus on general usefulness rather than aiming for the _absolute_ tryhard setup.  
Let me know if you think anything should be changed.

Current definitions can be viewed using the [interactive mapping tool](https://riotcoding.com/gw2/prepalert/mapping/).

## Options

> [!TIP]
> Hovering the `(?)` in-game will show additional information.

<img src="./docs/options.png?" alt="Prep Alert Options" />

### Positioning

For simple positioning, the _overlay anchor_ is synced with the _overlay content origin_.  
This means that if you move the overlay for example to the bottom-right corner of your screen, the reminders will also expand from the bottom-right corner, ensuring they are always visible.

If you want more control, you can disable syncing and adjust the anchor and origin separately.

The X and Y offset sliders can be clicked and dragged to quickly adjust the position. They specify the distance in pixels from the selected anchor point.

## Limitations

- Detection is position-based only and does not account for:
  - Boss phases or mechanics
  - Selected dungeon paths
  - Normal vs. challenge mode fractals

## 📥 Installation

> [!Caution]   
> Modifying Guild Wars 2 through any third-party software is not supported by ArenaNet.  
> Use this addon at your own risk.

#### You can install the addon directly through the in-game Nexus library with a single click.

Manual installation:

1. Download the latest [`PrepAlert.dll`](https://github.com/mriot/prep-alert/releases/latest/download/PrepAlert.dll)
2. Put the file into your Guild Wars 2 Nexus addons folder (e.g., `C:/Program Files/Guild Wars 2/addons`)  
3. Enable the addon in-game in Nexus

## Technical Stuff

The addon is written in C++20 for the [Nexus addon platform](https://raidcore.gg/Nexus).  
The scripts for map data fetching and processing are written in Python 3.11.

> [!Note]
> In order to have the addon open source, some sensitive parts were moved to a closed-source dependency.  

### Memory Reading ⚠️

A small portion of the addon relies on memory reading to

- retrieve currently active buffs on the player
- get the current map floor the player is on

### Map Data

Map data, originally fetched from the GW2 Wiki, is processed by [the map data patcher](./scripts/map_data_patcher/main.py) to produce `src/maps.json`.  
During this step, the patcher updates sector definitions by adding buff information, or creating additional sectors where necessary.

Custom sectors were created using an [interactive mapping tool](https://riotcoding.com/gw2/prepalert/mapping/).

> Currently, custom floors for some maps are not displayed correctly in the mapping tool

### Dependencies

- [Nexus](https://raidcore.gg/Nexus)
- [ImGui](https://github.com/ocornut/imgui)
- [nlohmann/json](https://github.com/nlohmann/json)

## Credits

- Raidcore's Nexus addon platform and community (especially Delta)
- The Gw2 Wiki, for a usable map data API and map widgets
- The folks who created the [Instanced Species List](https://docs.google.com/spreadsheets/d/135Iac7sWfB55kn3kFXWULneu4VfqmK8JqCHCrx3_ZMo/)
- Elis, Baste, Everless, and especially Bird for support and testing
- The `[ty]` guild and especially Devin for all stuff regarding Dungeons ♡
