#!/usr/bin/env python3
"""
Remove Box2D physics sources from NarutoSenKi mac hello Xcode project.
external/Box2D is not in this repo; the template pointed at ../../../external/Box2D.

Removes:
1) PBXGroup blocks for Box2D subtree (Collision, Shapes, Common, Dynamics, Contacts, Joints, Rope, root)
2) Remaining PBX lines whose object IDs use Box2D Xcode stamp 1683159D/1683159E

Run from repo root:
  python3 tools/strip_box2d_from_hello_mac_xcode.py
"""
from __future__ import annotations

import pathlib

_BOX2D_GROUP_KEYS = (
    "\t\t15C1573F1683159D00D239F2 /* Rope */ = {\n",
    "\t\t15C157151683159D00D239F2 /* Joints */ = {\n",
    "\t\t15C157021683159D00D239F2 /* Contacts */ = {\n",
    "\t\t15C156F41683159D00D239F2 /* Dynamics */ = {\n",
    "\t\t15C156E61683159D00D239F2 /* Common */ = {\n",
    "\t\t15C156DC1683159D00D239F2 /* Shapes */ = {\n",
    "\t\t15C156CE1683159D00D239F2 /* Collision */ = {\n",
    "\t\t15C156CB1683159D00D239F2 /* Box2D */ = {\n",
)

# Unique to Box2D fileRef + buildFile UUIDs in this template.
_BOX2D_SUFFIXES = ("1683159D00D239F2", "1683159E00D239F2")


def _strip_group_block(text: str, key: str) -> str:
    i = text.find(key)
    if i == -1:
        return text
    brace0 = text.index("{", i, i + len(key) + 8)
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
        raise RuntimeError(f"unclosed PBXGroup for key: {key[:50]!r}…")
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

    for key in _BOX2D_GROUP_KEYS:
        text = _strip_group_block(text, key)

    lines = text.splitlines(True)
    text = "".join(ln for ln in lines if not any(s in ln for s in _BOX2D_SUFFIXES))
    if text.count("{") != text.count("}"):
        raise SystemExit(
            f"Refusing to write {pbx}: brace mismatch after strip "
            f"({text.count('{')} vs {text.count('}')})."
        )
    pbx.write_text(text, encoding="utf-8")
    print(f"Patched {pbx.relative_to(root)} (removed Box2D Xcode refs)")


if __name__ == "__main__":
    main()
