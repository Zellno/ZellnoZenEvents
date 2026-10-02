# Zellno Zen Events

Zellno Zen Events is a clean-room DayZ addon that displays active vanilla
Chernarus events on Zens Map Enhancement.

It observes the event objects that actually exist in the running world and
creates one owned Zen Map marker for each logical occurrence. It does not alter
the Central Economy, event lifetimes or loot.

## Tracked events

- Helicrash
- Military Convoy
- Police Car
- Police Situation
- Train Wreck
- Airplane Crates
- Dynamic contaminated areas
- Permanent contaminated areas at Rify and Pavlovo

## Features

- one marker per logical event occurrence;
- private administrator-only monitoring;
- optional global markers for ordinary players;
- persistent per-administrator visibility preferences;
- personal administrator OFF overrides global ON;
- deterministic activation/deactivation filtering;
- private chat commands through ZenModCore;
- no dependency on VanillaPlusPlusMap;
- no reused code from the former Zellno Event Manager.

## Commands

Authorized administrators can use:

```text
!zze
!zze admin on
!zze admin off
!zze global on
!zze global off
!zze status admin
!zze status global
```

Recognized commands are handled privately and are not broadcast as ordinary
chat messages.

## Server configuration

On first successful server initialization, the addon creates:

```text
$profile:ZellnoZenEvents/ZellnoZenEventsConfig.json
```

Example:

```json
{
    "ConfigVersion": 2,
    "Enabled": true,
    "VisibilityMode": "AdministratorsOnly",
    "GlobalMarkersEnabled": false,
    "AdministratorSteamIds": [
        "76561198000000001"
    ],
    "AdministratorPreferences": []
}
```

Replace the example value with each administrator's SteamID64. Global markers
are disabled by default. Restart the server after manually editing the file.

## Requirements

- [Zens Core Mod](https://steamcommunity.com/sharedfiles/filedetails/?id=3702420204)
- [Zens Map Enhancement](https://steamcommunity.com/sharedfiles/filedetails/?id=3483440991)
- Zellno Zen Events must be loaded by both the server and connecting clients.

Zens Map Enhancement and ZenModCore are the addon's direct dependencies. CF is
a transitive dependency required by ZenModCore and must also be available in
the server/client mod stack.

## Installation

1. Load CF, ZenModCore and Zens Map Enhancement before Zellno Zen Events.
2. Add Zellno Zen Events to the client-visible server mod list.
3. Copy the included public key to the server `keys` directory.
4. Start the server once, stop it, then configure administrator SteamID64 values.

No wipe or mission reset is required.

## Building and testing

Run the automated tests:

```bash
python3 -m unittest discover -s tests -v
```

Build an unsigned deterministic candidate with `armake2`:

```bash
./build.sh
```

The build stays under `build/candidate/`; it does not sign, install or upload
the addon. See [docs/LOCAL_BUILD.md](docs/LOCAL_BUILD.md) and
[TESTING.md](TESTING.md).

## Architecture and licensing

- [Architecture](ARCHITECTURE.md)
- [Third-party notice](THIRD_PARTY.md)
- [Changelog](CHANGELOG.md)
- [Contributing](CONTRIBUTING.md)
- [MIT License](LICENSE)

This repository contains original source and documentation. It does not
distribute compiled PBOs, signatures, keys, private server profiles or hosting
data.

## Support

- [Zellno Mod Support — Discord](https://discord.gg/bhfBetKtqr)
- [Buy Me a Coffee — Noob Open Source](https://www.buymeacoffee.com/noobopensource)

## Disclaimer

This is an unofficial community modification for DayZ. It is not affiliated
with, authorized by, or endorsed by Bohemia Interactive a.s. DAYZ is a
registered trademark of Bohemia Interactive a.s.
