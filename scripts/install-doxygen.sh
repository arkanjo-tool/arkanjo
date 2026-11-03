#!/usr/bin/env bash
set -euo pipefail

DOXYGEN_VERSION="${1:?Usage: install-doxygen.sh <version> [SHA256]}"
DOXYGEN_SHA256="${2:-}"

DOXYGEN_TAG="Release_${DOXYGEN_VERSION//./_}"
ARCHIVE="doxygen-${DOXYGEN_VERSION}.linux.bin.tar.gz"
URL="https://github.com/doxygen/doxygen/releases/download/${DOXYGEN_TAG}/${ARCHIVE}"

TEMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TEMP_DIR"' EXIT

cd "$TEMP_DIR"

wget "$URL"

if [[ -n "$DOXYGEN_SHA256" ]]; then
    echo "${DOXYGEN_SHA256}  ${ARCHIVE}" | sha256sum --check
fi

tar -xzf "$ARCHIVE"

cd "doxygen-${DOXYGEN_VERSION}"
make install

INSTALLED_VERSION="$(doxygen --version | cut -d ' ' -f 1)"

if [[ "$INSTALLED_VERSION" != "$DOXYGEN_VERSION" ]]; then
    echo "Expected Doxygen ${DOXYGEN_VERSION}, got ${INSTALLED_VERSION}" >&2
    exit 1
fi