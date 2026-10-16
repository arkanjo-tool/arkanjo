#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CONFIG_FILE="$ROOT_DIR/config.json"

BUILD=false
CMAKE_ARGS=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        --build)
            BUILD=true
            shift
            ;;
        *)
            CMAKE_ARGS+=("$1")
            shift
            ;;
    esac
done

if ! command -v python3 >/dev/null 2>&1; then
    echo "Error: python3 not found."
    exit 1
fi

if ! METHODS=$(python3 "$ROOT_DIR/scripts/get_methods.py" "$CONFIG_FILE"); then
    echo "Failed to read '$CONFIG_FILE'."
    exit 1
fi

if [ -z "$METHODS" ]; then
    echo "No methods configured."
    echo "See 'docs/methods' for available methods and configuration" \
         "instructions."
    echo "Continuing without installing Python dependencies."

    if [ "$BUILD" = false ]; then
        exit 0
    fi
fi

INSTALL=false

for METHOD in $METHODS; do
    if [ -f "$ROOT_DIR/requirements/$METHOD.txt" ]; then
        INSTALL=true
        break
    fi
done

if [ "$INSTALL" = false ]; then
    echo "No Python dependencies required for the configured methods."
else
    for METHOD in $METHODS; do
        REQUIREMENTS="$ROOT_DIR/requirements/$METHOD.txt"

        if [ -f "$REQUIREMENTS" ]; then
            echo "Installing Python dependencies for '$METHOD'..."
            python3 -m pip install -r "$REQUIREMENTS"
        else
            echo "Method '$METHOD' does not require Python."
        fi
    done
fi

echo
echo "Bootstrap completed successfully."

if [ "$BUILD" = true ]; then
    echo
    echo "Configuring project..."
    cmake -B "$ROOT_DIR/build" "${CMAKE_ARGS[@]}"

    echo
    echo "Building project..."
    cmake --build "$ROOT_DIR/build"

    echo
    echo "Build completed successfully."
fi

echo

if [ -n "${VIRTUAL_ENV:-}" ]; then
    echo "Python environment:"
    echo "  $VIRTUAL_ENV"
elif [ -n "${CONDA_DEFAULT_ENV:-}" ]; then
    echo "Conda environment:"
    echo "  $CONDA_DEFAULT_ENV"
else
    echo "Make sure the Python environment with the required dependencies is active before running ArKanjo."
fi
