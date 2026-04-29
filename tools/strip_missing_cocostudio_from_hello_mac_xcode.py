#!/usr/bin/env python3
"""
Remove CocoStudio sub-modules that are NOT in this repo from the mac hello Xcode project.

Missing (extensions/CocoStudio/ does NOT contain):
  - Reader/         (WidgetReader, ButtonReader, …, GUIReader, SceneReader)
  - ActionTimeline/ (CCActionTimeline, CCFrame, …)
  - Trigger/        (TriggerBase, TriggerMng, TriggerObj, ObjectFactory)

Present (kept):
  - Armature/
  - GUI/

Run from repo root:
  python3 tools/strip_missing_cocostudio_from_hello_mac_xcode.py
"""
from __future__ import annotations

import pathlib
import re

_PBX = pathlib.Path(
    __file__
).resolve().parents[1] / "projects/NarutoSenki/proj.mac/hello.xcodeproj/project.pbxproj"

# Root PBXGroup UUIDs for the three missing sub-trees.
_ROOT_GROUPS = [
    "37C62C0C18E157C200D16FC4",  # Action
    "37C62C4718E157C200D16FC4",  # Components
    "37C62C9418E157C200D16FC4",  # Reader
    "2964940019518DD100BE20D8",  # ActionTimeline
    "37C62CC418E157C300D16FC4",  # Trigger
]

# Additional orphan references not covered by missing group roots.
_EXTRA_STRIP_UUIDS = {
    "37C62CDE18E157C300D16FC4",  # CCColliderDetector.cpp in Sources (PBXBuildFile)
    "37C62C3618E157C200D16FC4",  # CCColliderDetector.cpp (PBXFileReference)
    "37C62C3718E157C200D16FC4",  # CCColliderDetector.h (PBXFileReference)
}


def _get_block_extent(text: str, uuid: str) -> tuple[int, int] | tuple[None, None]:
    """Return (start, end) of the PBXGroup block for *uuid* (end exclusive, includes trailing newline)."""
    pat = uuid + r" /\* [^*]+ \*/ = \{"
    m = re.search(pat, text)
    if not m:
        return None, None
    start = m.start()
    open_brace = text.index("{", start)
    depth, k = 0, open_brace
    while k < len(text):
        if text[k] == "{":
            depth += 1
        elif text[k] == "}":
            depth -= 1
            if depth == 0:
                k += 1
                if k < len(text) and text[k] == ";":
                    k += 1
                if k < len(text) and text[k] == "\n":
                    k += 1
                break
        k += 1
    return start, k


def _is_group(text: str, uuid: str) -> bool:
    return bool(re.search(uuid + r" /\* [^*]+ \*/ = \{\n\t\t\tisa = PBXGroup", text))


def _collect_uuids(original_text: str, uuid: str) -> set[str]:
    """Recursively collect all UUIDs referenced inside the PBXGroup block tree."""
    visited: set[str] = set()
    queue = [uuid]
    while queue:
        uid = queue.pop()
        if uid in visited:
            continue
        visited.add(uid)
        s, e = _get_block_extent(original_text, uid)
        if s is None:
            continue
        for child in re.findall(r"[0-9A-F]{24}", original_text[s:e]):
            if child not in visited:
                queue.append(child)
    return visited


def _recursive_strip_groups(text: str, root_uuid: str) -> str:
    """Strip nested PBXGroup blocks from deepest to root."""
    s, e = _get_block_extent(text, root_uuid)
    if s is None:
        return text
    block = text[s:e]
    for child in re.findall(r"[0-9A-F]{24}", block):
        if child != root_uuid and _is_group(text, child):
            text = _recursive_strip_groups(text, child)
    s, e = _get_block_extent(text, root_uuid)
    if s is None:
        return text
    return text[:s] + text[e:]


def main() -> None:
    root = pathlib.Path(__file__).resolve().parents[1]
    text = _PBX.read_text(encoding="utf-8")
    original = text

    # Collect all UUIDs to strip (for later line-filter pass)
    all_strip_uuids: set[str] = set()
    for uuid in _ROOT_GROUPS:
        all_strip_uuids |= _collect_uuids(original, uuid)
    all_strip_uuids |= _EXTRA_STRIP_UUIDS

    # Pass 1: remove PBXGroup blocks (balanced-brace)
    for uuid in _ROOT_GROUPS:
        text = _recursive_strip_groups(text, uuid)

    # Pass 2: strip residual lines in PBXBuildFile / PBXFileReference / Sources
    lines = text.splitlines(True)
    text = "".join(ln for ln in lines if not any(u in ln for u in all_strip_uuids))

    opens, closes = text.count("{"), text.count("}")
    if opens != closes:
        raise SystemExit(
            f"Refusing to write: brace mismatch after strip ({opens} open vs {closes} close). "
            "Restore from git and investigate."
        )

    _PBX.write_text(text, encoding="utf-8")
    removed_uuids = len(all_strip_uuids)
    print(
        f"Patched {_PBX.relative_to(root)} "
        f"(stripped missing CocoStudio modules and orphans — {removed_uuids} UUIDs)"
    )


if __name__ == "__main__":
    main()
