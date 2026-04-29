#!/usr/bin/env python3
"""
Remove bundled Chipmunkphysics sources from NarutoSenki mac hello Xcode project.
external/chipmunk is not in this repo; the hello template referenced it + CMakeLists.txt in Resources.

Run from repo root:
  python3 tools/strip_chipmunk_from_hello_mac_xcode.py
"""
from __future__ import annotations

import pathlib


def main() -> None:
    root = pathlib.Path(__file__).resolve().parents[1]
    pbx = root / "projects/NarutoSenki/proj.mac/hello.xcodeproj/project.pbxproj"
    lines = pbx.read_text(encoding="utf-8").splitlines(True)
    out: list[str] = []
    for ln in lines:
        if "168315B300D239F2" in ln:
            continue
        if "external/chipmunk" in ln:
            continue
        if ln.strip() == "CC_ENABLE_CHIPMUNK_INTEGRATION,":
            continue
        out.append(ln)

    pbx.write_text("".join(out), encoding="utf-8")
    print(f"Patched {pbx.relative_to(root)} (removed Chipmunk Xcode refs)")


if __name__ == "__main__":
    main()
