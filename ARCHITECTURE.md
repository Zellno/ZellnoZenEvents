# Architecture

Zellno Zen Events is a clean-room DayZ addon that detects active vanilla
Chernarus events and presents them through the public Zen Map marker API.

## Detection

The server scans reviewed vanilla event positions with `GetObjectsAtPosition`.
Each candidate accepts only exact runtime classnames from its semantic-anchor
allowlist. Detected `Object` references are not retained.

Activation and deactivation each require two consecutive matching observations.
This avoids marker flicker while keeping the Central Economy authoritative.

The runtime processes at most 28 candidates per tick. The 562 logical
candidates are spread across 21 one-second ticks after an initial ten-second
startup delay.

## Event consolidation

Physical objects that belong to one logical occurrence are consolidated before
presentation:

- 28 possible Airplane Crate positions become two spatial occurrences;
- four static contaminated volumes become the Rify occurrence;
- five static contaminated volumes become the Pavlovo occurrence.

The addon therefore creates one marker per logical event, not one marker per
component object.

## Marker ownership

Every marker created by the addon is retained by its owned candidate ID. Marker
removal uses only the stored `MapMarker` object. The addon never clears Zen Map
markers generically and never removes markers by name, position or array index.

Administrator markers are player-specific. Global delivery is optional.
Personal administrator OFF takes precedence over global ON.

## Configuration and commands

The server stores configuration under
`$profile:ZellnoZenEvents/ZellnoZenEventsConfig.json`.

ZenModCore's chat-command chain exposes only the `!zze` namespace. The addon
does not modify ZellnoGlobalChat or VPPAdminTools.

## Dependencies

Direct addon requirements are `DZ_Data`, `DZ_Scripts`, `ZenModCore` and
`ZenMap`. Zens Map Enhancement requires ZenModCore, and ZenModCore requires CF.

No Zen Map, ZenModCore, CF, DayZ or third-party source or asset is redistributed.
