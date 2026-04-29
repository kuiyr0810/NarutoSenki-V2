#!/usr/bin/env python3
"""
Patch projects/NarutoSenki/proj.mac/hello.xcodeproj/project.pbxproj to bundle
the runtime game assets and Lua scripts:

  - Adds the on-disk folders projects/NarutoSenki/Resources/ and
    projects/NarutoSenki/lua/ as Xcode "folder references" (the blue
    folders) so Xcode copies their contents verbatim into the .app bundle
    under Contents/Resources/Resources/ and Contents/Resources/lua/.
  - Adds matching entries to the existing PBXResourcesBuildPhase so they
    actually get copied during build.
  - Inserts the references into the existing top-level "Resources" group
    so they show up under the project navigator.

The patch is idempotent: re-running it after success is a no-op.

Usage (from repository root):
  python3 tools/add_mac_bundle_resources.py
"""
from __future__ import annotations

import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[1]
PBX = ROOT / "projects/NarutoSenki/proj.mac/hello.xcodeproj/project.pbxproj"

# Stable, hand-picked 24-char hex IDs that are unlikely to collide with
# anything Xcode auto-generates. They follow Xcode's hex pattern.
FOLDER_REF_RES = "AABBCC0001000000DEADBE01"
FOLDER_REF_LUA = "AABBCC0001000000DEADBE02"
BUILD_REF_RES  = "AABBCC0002000000DEADBE01"
BUILD_REF_LUA  = "AABBCC0002000000DEADBE02"

# Existing markers we rely on inside the pbxproj.
RESOURCES_GROUP_MARKER = (
    "\t\t78C7DDAA14EBA5050085D0C2 /* Resources */ = {\n"
    "\t\t\tisa = PBXGroup;\n"
    "\t\t\tchildren = (\n"
)

RESOURCES_PHASE_MARKER = (
    "\t\t1D60588D0D05DD3D006BFB54 /* Resources */ = {\n"
    "\t\t\tisa = PBXResourcesBuildPhase;\n"
    "\t\t\tbuildActionMask = 2147483647;\n"
    "\t\t\tfiles = (\n"
)


def main() -> None:
    text = PBX.read_text(encoding="utf-8")

    if FOLDER_REF_RES in text:
        print("Naruto mac bundle resources patch already applied (skip).")
        return

    # 1. Add file references for the two folders. Xcode treats
    #    `lastKnownFileType = folder` as a "folder reference" (blue folder)
    #    so the entire on-disk directory is recursively copied verbatim
    #    into the produced .app bundle.
    file_refs = (
        f"\t\t{FOLDER_REF_RES} /* Resources */ = "
        f"{{isa = PBXFileReference; lastKnownFileType = folder; "
        f"name = Resources; path = ../Resources; sourceTree = \"<group>\"; }};\n"
        f"\t\t{FOLDER_REF_LUA} /* lua */ = "
        f"{{isa = PBXFileReference; lastKnownFileType = folder; "
        f"name = lua; path = ../lua; sourceTree = \"<group>\"; }};\n"
    )
    text = text.replace(
        "/* End PBXFileReference section */\n",
        file_refs + "/* End PBXFileReference section */\n",
        1,
    )

    # 2. Add matching PBXBuildFile entries so they appear in the
    #    Resources copy phase.
    build_files = (
        f"\t\t{BUILD_REF_RES} /* Resources in Resources */ = "
        f"{{isa = PBXBuildFile; fileRef = {FOLDER_REF_RES} /* Resources */; }};\n"
        f"\t\t{BUILD_REF_LUA} /* lua in Resources */ = "
        f"{{isa = PBXBuildFile; fileRef = {FOLDER_REF_LUA} /* lua */; }};\n"
    )
    text = text.replace(
        "/* End PBXBuildFile section */\n",
        build_files + "/* End PBXBuildFile section */\n",
        1,
    )

    # 3. Show the references in the top-level Resources project group.
    if RESOURCES_GROUP_MARKER not in text:
        raise SystemExit("Resources project group marker not found.")
    text = text.replace(
        RESOURCES_GROUP_MARKER,
        RESOURCES_GROUP_MARKER
        + f"\t\t\t\t{FOLDER_REF_RES} /* Resources */,\n"
        + f"\t\t\t\t{FOLDER_REF_LUA} /* lua */,\n",
        1,
    )

    # 4. And, most importantly, register them in the Resources build
    #    phase so they actually get copied at build time.
    if RESOURCES_PHASE_MARKER not in text:
        raise SystemExit("Resources build phase marker not found.")
    text = text.replace(
        RESOURCES_PHASE_MARKER,
        RESOURCES_PHASE_MARKER
        + f"\t\t\t\t{BUILD_REF_RES} /* Resources in Resources */,\n"
        + f"\t\t\t\t{BUILD_REF_LUA} /* lua in Resources */,\n",
        1,
    )

    PBX.write_text(text, encoding="utf-8")
    print(f"Updated {PBX.relative_to(ROOT)}")
    print(
        "Resources/ and lua/ are now copied into the .app bundle as folder\n"
        "references. AppDelegate adds 'Contents/Resources/lua/' and the\n"
        "extra 'Contents/Resources/Resources/' search path on macOS so the\n"
        "engine can resolve sprites and Lua scripts at runtime."
    )


if __name__ == "__main__":
    main()
