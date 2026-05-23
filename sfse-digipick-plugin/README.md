# EasyDigipick SFSE Plugin

Runtime digipick difficulty overrides for Starfield via SFSE.

## What It Does

This plugin writes compatible `uSecurity*` GMST values at `kDataLoaded`, so the settings apply even when old ESM IDs were removed by game updates.

Currently targeted IDs (Starfield 1.16.242):

- uSecurityKeyMinSizeNovice
- uSecurityKeyMinSizeAverage
- uSecurityKeyMinSizeHard
- uSecurityKeyMinSizeVeryHard
- uSecurityKeyMaxSizeNovice
- uSecurityMaxKeysNovice
- uSecurityMaxKeysAverage
- uSecurityMaxKeysHard
- uSecurityMaxKeysVeryHard
- uSecurityMaxKeysPerRingNovice
- uSecurityMaxKeysPerRingVeryHard
- uSecurityPuzzleShapeCountAverage
- uSecurityPuzzleShapeCountHard

## Build

Requirements:

- CMake 3.26+
- Visual Studio 2022 Build Tools (MSVC)
- Git

Steps (run on Windows, native or your usual SFSE build VM):

1. `cd sfse-digipick-plugin`
2. `cmake -S . -B build -G "Visual Studio 17 2022" -A x64`
3. `cmake --build build --config Release`

Expected output:

- `Data/SFSE/Plugins/EasyDigipickSFSE.dll`

## Deploy

1. Copy `Data/SFSE/Plugins/EasyDigipickSFSE.dll` to your live game path:
   - `<Starfield>/Data/SFSE/Plugins/`
2. Copy `config/EasyDigipickSFSE.ini` to:
   - `<Starfield>/Data/SFSE/Plugins/EasyDigipickSFSE.ini`
3. Launch with SFSE loader.

## Logs

Plugin log path:

- `<My Games>/Starfield/SFSE/Logs/EasyDigipickSFSE.log`

## Notes

- This plugin is designed to complement or replace ESM-based digipick tweaks that relied on removed GMST IDs.
- If CommonLibSF API changes, update include and setting access calls in `src/main.cpp`.
