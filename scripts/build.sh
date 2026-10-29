#!/usr/bin/env bash
# Zucks Server - Build Script
# (c) 2026 Zucks Project

set -euo pipefail

BUILD_DIR="build"
BUILD_TYPE="${1:-Release}"

echo "[Zucks] Configuring build (${BUILD_TYPE})..."
cmake -S . -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DZUCKS_BUILD_TESTS=ON

echo "[Zucks] Building..."
cmake --build "${BUILD_DIR}" -j"$(nproc 2>/dev/null || echo 4)"

echo "[Zucks] Build complete -> ${BUILD_DIR}/zucks-server"