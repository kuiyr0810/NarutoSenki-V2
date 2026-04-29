#!/usr/bin/env python3
"""
Remove Dear ImGui (external/imgui) from cocos2dx macOS Xcode project.
NarutoSenki disables ImGui in EAGLView.mm (CC_USE_IMGUI = 0); external/imgui is not in this repo.

Run from repo root:
  python3 tools/strip_imgui_from_cocos2dx_mac_xcode.py
"""
from __future__ import annotations

import pathlib


def main() -> None:
    root = pathlib.Path(__file__).resolve().parents[1]
    pbx = root / "cocos2dx/proj.mac/cocos2dx.xcodeproj/project.pbxproj"
    lines = pbx.read_text(encoding="utf-8").splitlines(keepends=True)
    out: list[str] = []
    i = 0
    while i < len(lines):
        ln = lines[i]
        if "84911DE6250DB78600D14782 /* imgui */" in ln and " = {" in ln:
            depth = 0
            start = i
            j = i
            while j < len(lines):
                for ch in lines[j]:
                    if ch == "{":
                        depth += 1
                    elif ch == "}":
                        depth -= 1
                j += 1
                if j > start and depth == 0:
                    break
            i = j
            continue
        out.append(ln)
        i += 1

    text = "".join(out)
    text = text.replace("MACOSX_DEPLOYMENT_TARGET = 10.10;", "MACOSX_DEPLOYMENT_TARGET = 11.0;")
    if text.count("{") != text.count("}"):
        raise SystemExit(
            f"Refusing to write {pbx}: brace mismatch after patch "
            f"({text.count('{')} open vs {text.count('}')} close). Restore project.pbxproj from git."
        )
    pbx.write_text(text, encoding="utf-8")
    print(f"Patched {pbx.relative_to(root)}")


if __name__ == "__main__":
    main()
