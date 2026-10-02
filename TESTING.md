# Testing

Version 0.1.0 was validated locally with:

- 138 automated source, architecture, manifest and release-integrity tests;
- deterministic PBO reproduction;
- successful PBO signature verification;
- successful Linux server and client loading with the complete local mod stack;
- Zen Map and ZenModCore integration;
- VPPAdminTools teleport verification of marker/object parity;
- one marker per logical event occurrence;
- Airplane Crate consolidation from 21 + 3 observed objects into two markers;
- consolidation of nine static contaminated volumes into Rify and Pavlovo;
- administrator-only, global and personal-override visibility states;
- private `!zze` command handling;
- configuration persistence across server and client restarts.

The pre-publication homologated candidate PBO SHA-256 was:

```text
a596dc90121598729d0fe8a243fecf1fdbefa3d2dffbfa724b5671b4c4be16b5
```

The final public source removes unused mission-specific generation metadata.
Its release PBO must therefore be rebuilt, signed and smoke-tested before Steam
Workshop publication.

Private profiles, Steam IDs, production coordinates, logs and hosting data are
excluded from the repository.
