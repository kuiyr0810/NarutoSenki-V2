#!/usr/bin/env python3
"""
Remove Spine extension sources from NarutoSenKi mac hello Xcode project.
extensions/spine is not in this repo (no spine-cocos2dx.cpp, CCSkeleton*, etc.).

Removes:
1) PBXGroup block for spine (balanced braces)
2) Remaining PBX lines whose IDs use the spine CCSkeleton/spine-runtime suffixes

Run from repo root:
  python3 tools/strip_spine_from_hello_mac_xcode.py
"""
from __future__ import annotations

import pathlib


_SPINE_SUFFIXES = ("1727BFC6006D4861", "175E0AFA005D39BF")


def _strip_spine_group_block(text: str) -> str:
    key = "\t\t1A40E7331727BFC6006D4861 /* spine */ = {\n"
    i = text.find(key)
    if i == -1:
        return text
    brace0 = text.index("{", i, i + len(key) + 5)
    depth = 0
    k = brace0
    while k < len(text):
        ch = text[k]
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0:
                k += 1
                if k < len(text) and text[k] == ";":
                    k += 1
                break
        k += 1
    else:
        raise RuntimeError("unclosed spine PBXGroup block")
    # Optional newline after }; 
    nl = ""
    if k < len(text) and text[k] == "\r":
        k += 1
    if k < len(text) and text[k] == "\n":
        nl = text[k]
        k += 1
    return text[:i] + nl + text[k:]


def main() -> None:
    root = pathlib.Path(__file__).resolve().parents[1]
    pbx = root / "projects/NarutoSenki/proj.mac/hello.xcodeproj/project.pbxproj"
    text = pbx.read_text(encoding="utf-8")

    text = _strip_spine_group_block(text)
    lines = text.splitlines(True)
    out: list[str] = []
    for ln in lines:
        if any(s in ln for s in _SPINE_SUFFIXES):
            continue
        out.append(ln)
    text = "".join(out)
    if text.count("{") != text.count("}"):
        raise SystemExit(
            f"Refusing to write {pbx}: brace mismatch after strip "
            f"({text.count('{')} vs {text.count('}')})."
        )
    pbx.write_text(text, encoding="utf-8")
    print(f"Patched {pbx.relative_to(root)} (removed Spine extension refs)")


if __name__ == "__main__":
    main()
