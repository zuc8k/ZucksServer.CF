#!/usr/bin/env bash
# Zucks Server - Run Script
# (c) 2026 Zucks Project

set -euo pipefail

CONFIG_PATH="${1:-config/zucks_config.json}"
BIN="./build/zucks-server"

if [[ ! -x "${BIN}" ]]; then
    echo "[Zucks] Binary not found. Run scripts/build.sh first." >&2
    exit 1
fi

if [[ ! -f "${CONFIG_PATH}" ]]; then
    echo "[Zucks] Config not found: ${CONFIG_PATH}" >&2
    exit 1
fi

echo "[Zucks] Starting with config: ${CONFIG_PATH}"
exec "${BIN}" "${CONFIG_PATH}"