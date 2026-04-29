#!/usr/bin/env bash
# Build LuaJIT as a static archive for the macOS NarutoSenki Xcode target.
# Expects LuaJIT sources at scripting/lua/luajit/LuaJIT (clone from https://github.com/LuaJIT/LuaJIT)
set -euo pipefail

# LuaJIT's Makefile requires this on current Xcode (otherwise: "missing: export MACOSX_DEPLOYMENT_TARGET=XX.YY")
: "${MACOSX_DEPLOYMENT_TARGET:=11.0}"
export MACOSX_DEPLOYMENT_TARGET

HERE="$(cd "$(dirname "$0")" && pwd)"
DEST="$HERE/mac"
LJ="$HERE/LuaJIT"

mkdir -p "$DEST"

if [[ ! -d "$LJ" ]]; then
	echo "Clone LuaJIT into: $LJ"
	echo "  git clone https://github.com/LuaJIT/LuaJIT.git \"$LJ\""
	exit 1
fi

(cd "$LJ" && make clean && make amalg)
cp -f "$LJ/src/libluajit.a" "$DEST/libluajit.a"
echo "Installed $DEST/libluajit.a"
