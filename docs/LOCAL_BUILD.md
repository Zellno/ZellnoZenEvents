# Isolated local candidate build

This procedure creates an unsigned candidate PBO only inside the project. It
does not copy files to the DayZ server, client, keys directory or Workshop.

## Requirements

- Linux with Bash;
- `armake2` available in `PATH`;
- the complete Zellno Zen Events source tree.

The release was validated with `armake2` version `0.3.0`.

## Command

From the project root:

```bash
chmod +x build.sh tools/build_candidate.sh
./build.sh
```

The script:

1. rejects a missing tool or source input;
2. stages only `config.cpp`, `$PBOPREFIX$` and `scripts/` in a temporary
   directory;
3. rapifies `config.cpp` separately;
4. builds the PBO twice;
5. rejects non-identical build outputs;
6. inspects the accepted PBO;
7. writes only these candidate artifacts:

```text
build/candidate/ZellnoZenEvents.pbo
build/candidate/ZellnoZenEvents.inspect.txt
build/candidate/ZellnoZenEvents.sha256
```

## Important limitation

Successful PBO creation proves configuration rapification and packaging. It
does not compile or execute Enforce Script. Script compilation is performed by
DayZ while loading the addon and remains a separate, controlled laboratory
gate.

Review the candidate before any signing or installation step; do not sign it
or install it as part of this build, and do not add it to the client mod line.
Private signing keys must never be committed to this repository.
