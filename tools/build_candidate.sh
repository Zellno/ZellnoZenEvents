#!/usr/bin/env bash

set -euo pipefail

export LC_ALL=C

PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd -P)"
OUTPUT_DIR="$PROJECT_DIR/build/candidate"

CONFIG_SOURCE="$PROJECT_DIR/config.cpp"
PREFIX_SOURCE="$PROJECT_DIR/\$PBOPREFIX\$"
SCRIPTS_SOURCE="$PROJECT_DIR/scripts"

if ! command -v armake2 >/dev/null 2>&1; then
    echo "ERRO: armake2 não foi encontrado no PATH." >&2
    exit 1
fi

for required_path in "$CONFIG_SOURCE" "$PREFIX_SOURCE" "$SCRIPTS_SOURCE"; do
    if [[ ! -e "$required_path" ]]; then
        printf 'ERRO: entrada obrigatória ausente: %s\n' "$required_path" >&2
        exit 1
    fi
done

mkdir -p -- "$OUTPUT_DIR"

TEMP_DIR="$(mktemp -d)"
trap 'rm -rf -- "$TEMP_DIR"' EXIT

STAGE_DIR="$TEMP_DIR/ZellnoZenEvents"
mkdir -p -- "$STAGE_DIR/scripts"

cp -- "$CONFIG_SOURCE" "$STAGE_DIR/config.cpp"
cp -- "$PREFIX_SOURCE" "$STAGE_DIR/\$PBOPREFIX\$"
cp -a -- "$SCRIPTS_SOURCE/." "$STAGE_DIR/scripts/"

armake2 rapify -f \
    "$STAGE_DIR/config.cpp" \
    "$TEMP_DIR/config.bin"

FIRST_PBO="$TEMP_DIR/ZellnoZenEvents-first.pbo"
SECOND_PBO="$TEMP_DIR/ZellnoZenEvents-second.pbo"

armake2 build -f "$STAGE_DIR" "$FIRST_PBO"
armake2 build -f "$STAGE_DIR" "$SECOND_PBO"

if ! cmp -s -- "$FIRST_PBO" "$SECOND_PBO"; then
    echo "ERRO: os dois builds produziram PBOs diferentes." >&2
    exit 1
fi

INSPECTION_FILE="$TEMP_DIR/ZellnoZenEvents.inspect.txt"
armake2 inspect "$FIRST_PBO" >"$INSPECTION_FILE"

CANDIDATE_PBO="$OUTPUT_DIR/ZellnoZenEvents.pbo"
CANDIDATE_INSPECTION="$OUTPUT_DIR/ZellnoZenEvents.inspect.txt"
CANDIDATE_HASH="$OUTPUT_DIR/ZellnoZenEvents.sha256"

install -m 0644 -- "$FIRST_PBO" "$CANDIDATE_PBO"
install -m 0644 -- "$INSPECTION_FILE" "$CANDIDATE_INSPECTION"
sha256sum "$CANDIDATE_PBO" >"$CANDIDATE_HASH"

printf '\nBuild candidato concluído sem instalação.\n'
printf 'PBO: %s\n' "$CANDIDATE_PBO"
printf 'Inspeção: %s\n' "$CANDIDATE_INSPECTION"
printf 'SHA-256: '
cut -d ' ' -f 1 "$CANDIDATE_HASH"
