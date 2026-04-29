#!/usr/bin/env python3
"""
Patch projects/NarutoSenki/proj.mac/hello.xcodeproj/project.pbxproj to:
- Remove broken HelloWorld references
- Add NarutoSenki game sources + Lua scripting sources (same scope as proj.linux / Windows)
- Append header / library search paths and link libluajit (build output in scripting/lua/luajit/mac/)

Run from repository root:
  python3 tools/patch_mac_xcode.py

Then build LuaJIT (see Doc/README.md macOS section), then:
  xcodebuild -project projects/NarutoSenki/proj.mac/hello.xcodeproj -scheme hello -configuration Debug
"""
from __future__ import annotations

import hashlib
import re
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[1]
PBX = ROOT / "projects/NarutoSenki/proj.mac/hello.xcodeproj/project.pbxproj"

GAME_SOURCES = [
    "ActionButton.cpp",
    "CharacterBase.cpp",
    "BGLayer.cpp",
    "CreditsLayer.cpp",
    "Effect.cpp",
    "Element.cpp",
    "GameLayer.cpp",
    "GameOver.cpp",
    "GameScene.cpp",
    "GearLayer.cpp",
    "HPBar.cpp",
    "HudLayer.cpp",
    "JoyStick.cpp",
    "LoadLayer.cpp",
    "PauseLayer.cpp",
    "SelectLayer.cpp",
    "StartMenu.cpp",
    "UI/GameModeLayer.cpp",
    "MyUtils/CCScrewLayer.cpp",
    "MyUtils/CCShake.cpp",
    "MyUtils/CCStrokeLabel.cpp",
    "MyUtils/KTools.cpp",
    "MyUtils/MD5ChecksumDefines.cpp",
]

LUA_SOURCES = [
    "tolua/tolua_event.c",
    "tolua/tolua_is.c",
    "tolua/tolua_map.c",
    "tolua/tolua_push.c",
    "tolua/tolua_to.c",
    "cocos2dx_support/tolua_fix.c",
    "cocos2dx_support/CCLuaBridge.cpp",
    "cocos2dx_support/CCLuaEngine.cpp",
    "cocos2dx_support/CCLuaStack.cpp",
    "cocos2dx_support/CCLuaValue.cpp",
    "cocos2dx_support/Cocos2dxLuaLoader.cpp",
    "cocos2dx_support/LuaCocos2d.cpp",
    "cocos2dx_support/LuaCocoStudio.cpp",
    "cocos2dx_support/lua_cocos2dx_manual.cpp",
    "cocos2dx_support/lua_cocos2dx_extensions_manual.cpp",
    "cocos2dx_support/lua_cocos2dx_cocostudio_manual.cpp",
    "xxtea/xxtea.cpp",
]


def make_id(seed: str, salt: str) -> str:
    h = hashlib.sha1((seed + "\0" + salt).encode()).hexdigest()
    return h[:24].upper()


def main() -> None:
    text = PBX.read_text(encoding="utf-8")

    if make_id("game", GAME_SOURCES[0]) in text:
        print("Naruto mac Xcode patch already applied (skip).")
        return

    # Remove HelloWorld file / build entries
    text = re.sub(
        r"\t\t1AFAF8CB16D3886000DB1158 /\* HelloWorldScene\.cpp in Sources \*/ = \{isa = PBXBuildFile[^}]+\};\n",
        "",
        text,
    )
    text = re.sub(
        r"\t\t1AFAF8C516D3884900DB1158 /\* HelloWorld\.png in Resources \*/ = \{isa = PBXBuildFile[^}]+\};\n",
        "",
        text,
    )
    text = re.sub(
        r"\t\t1AFAF8C816D3886000DB1158 /\* HelloWorldScene\.cpp \*/ = \{isa = PBXFileReference[^}]+\};\n",
        "",
        text,
    )
    text = re.sub(
        r"\t\t1AFAF8C916D3886000DB1158 /\* HelloWorldScene\.h \*/ = \{isa = PBXFileReference[^}]+\};\n",
        "",
        text,
    )
    text = re.sub(
        r"\t\t1AFAF8C216D3884900DB1158 /\* HelloWorld\.png \*/ = \{isa = PBXFileReference[^}]+\};\n",
        "",
        text,
    )
    text = re.sub(
        r"\t\t\t\t1AFAF8C816D3886000DB1158 /\* HelloWorldScene\.cpp \*/,\n",
        "",
        text,
    )
    text = re.sub(
        r"\t\t\t\t1AFAF8C916D3886000DB1158 /\* HelloWorldScene\.h \*/,\n",
        "",
        text,
    )
    text = re.sub(
        r"\t\t\t\t1AFAF8CB16D3886000DB1158 /\* HelloWorldScene\.cpp in Sources \*/,\n",
        "",
        text,
    )
    text = re.sub(
        r"\t\t\t\t1AFAF8C516D3884900DB1158 /\* HelloWorld\.png in Resources \*/,\n",
        "",
        text,
    )

    game_build_lines = []
    game_ref_lines = []
    game_group_lines = []
    game_source_phase_lines = []

    lua_build_lines = []
    lua_ref_lines = []
    lua_group_lines = []
    lua_source_phase_lines = []

    for rel in GAME_SOURCES:
        bid = make_id("game", rel)
        rid = make_id("gameref", rel)
        name = pathlib.Path(rel).name
        path = f"../Classes/{rel}"
        game_build_lines.append(
            f"\t\t{bid} /* {name} in Sources */ = {{isa = PBXBuildFile; fileRef = {rid} /* {name} */; }};"
        )
        game_ref_lines.append(
            f"\t\t{rid} /* {name} */ = {{isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = sourcecode.cpp.cpp; name = {name}; path = {path}; sourceTree = \"<group>\"; }};"
        )
        game_group_lines.append(f"\t\t\t\t{rid} /* {name} */,")
        game_source_phase_lines.append(f"\t\t\t\t{bid} /* {name} in Sources */,")

    LUA_PREFIX = "../../../scripting/lua"

    for rel in LUA_SOURCES:
        bid = make_id("lua", rel)
        rid = make_id("luaref", rel)
        name = pathlib.Path(rel).name
        lt = "sourcecode.c.c" if rel.endswith(".c") else "sourcecode.cpp.cpp"
        path = f"{LUA_PREFIX}/{rel}"
        lua_build_lines.append(
            f"\t\t{bid} /* {name} in Sources */ = {{isa = PBXBuildFile; fileRef = {rid} /* {name} */; }};"
        )
        lua_ref_lines.append(
            f"\t\t{rid} /* {name} */ = {{isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = {lt}; name = {name}; path = {path}; sourceTree = \"<group>\"; }};"
        )
        lua_group_lines.append(f"\t\t\t\t{rid} /* {name} */,")
        lua_source_phase_lines.append(f"\t\t\t\t{bid} /* {name} in Sources */,")

    insert_build = "\n".join(game_build_lines) + "\n" + "\n".join(lua_build_lines) + "\n"
    fw_build = (
        "\t\tMACCF000FACEFOUNDATION01 /* CoreFoundation.framework in Frameworks */ = "
        "{isa = PBXBuildFile; fileRef = MACCF000FACEFOUNDATIONREF01 /* CoreFoundation.framework */; };\n"
    )
    insert_build = fw_build + insert_build

    insert_ref = "\n".join(game_ref_lines) + "\n" + "\n".join(lua_ref_lines) + "\n"
    fw_ref = (
        "\t\tMACCF000FACEFOUNDATIONREF01 /* CoreFoundation.framework */ = "
        "{isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = CoreFoundation.framework; path = "
        "System/Library/Frameworks/CoreFoundation.framework; sourceTree = SDKROOT; };\n"
    )
    insert_ref = fw_ref + insert_ref

    text = text.replace(
        "/* Begin PBXBuildFile section */\n",
        "/* Begin PBXBuildFile section */\n" + insert_build,
        1,
    )

    text = text.replace(
        "/* End PBXFileReference section */\n",
        insert_ref + "\n/* End PBXFileReference section */\n",
        1,
    )

    marker = (
        "\t\t15AA9C4015B7EC450033D6C2 /* Classes */ = {\n"
        "\t\t\tisa = PBXGroup;\n"
        "\t\t\tchildren = (\n"
    )
    if marker not in text:
        raise SystemExit("Classes group marker not found; pbxproj format changed.")

    insert_group = "".join(game_group_lines) + "".join(lua_group_lines)
    text = text.replace(marker, marker + insert_group, 1)

    marker_src = (
        "\t\t1D60588E0D05DD3D006BFB54 /* Sources */ = {\n"
        "\t\t\tisa = PBXSourcesBuildPhase;\n"
        "\t\t\tbuildActionMask = 2147483647;\n"
        '\t\t\tfiles = (\n'
    )
    if marker_src not in text:
        raise SystemExit("Sources phase marker not found.")

    combined_sources = "".join(game_source_phase_lines) + "".join(lua_source_phase_lines)
    text = text.replace(marker_src, marker_src + combined_sources, 1)

    inc_line = '\t\t\t\t\t"$(SRCROOT)/../../../CocosDenshion/include",\n'
    inc_extra = (
        inc_line
        + '\t\t\t\t\t"$(SRCROOT)/../../../projects/NarutoSenki/Classes",\n'
        + '\t\t\t\t\t"$(SRCROOT)/../../../scripting/lua/luajit/include",\n'
        + '\t\t\t\t\t"$(SRCROOT)/../../../scripting/lua/tolua",\n'
        + '\t\t\t\t\t"$(SRCROOT)/../../../scripting/lua/cocos2dx_support",\n'
    )
    if "projects/NarutoSenki/Classes" not in text:
        text = text.replace(inc_line, inc_extra, 2)

    old_lib = '\t\t\t\tLIBRARY_SEARCH_PATHS = "\\"$(SRCROOT)/../../../cocos2dx/platform/third_party/mac/libraries\\"";\n'
    new_lib = (
        "\t\t\t\tLIBRARY_SEARCH_PATHS = (\n"
        '\t\t\t\t\t"$(SRCROOT)/../../../cocos2dx/platform/third_party/mac/libraries",\n'
        '\t\t\t\t\t"$(SRCROOT)/../../../scripting/lua/luajit/mac",\n'
        "\t\t\t\t);\n"
    )
    if "scripting/lua/luajit/mac" not in text:
        text = text.replace(old_lib, new_lib, 2)

    if "OTHER_LDFLAGS" not in text:
        text = text.replace(
            "\t\t\t\tMACOSX_DEPLOYMENT_TARGET = 10.10;\n",
            "\t\t\t\tMACOSX_DEPLOYMENT_TARGET = 10.10;\n"
            '\t\t\t\tOTHER_LDFLAGS = "-lluajit";\n',
            2,
        )

    fw_phase = (
        "\t\t\tfiles = (\n"
        "\t\t\t\tMACCF000FACEFOUNDATION01 /* CoreFoundation.framework in Frameworks */,\n"
        "\t\t\t\t1A9CE9A81765A889000E3062 /* libsqlite3.dylib in Frameworks */,\n"
    )
    if "MACCF000FACEFOUNDATION01" not in text:
        text = text.replace(
            "\t\t\tfiles = (\n\t\t\t\t1A9CE9A81765A889000E3062 /* libsqlite3.dylib in Frameworks */,\n",
            fw_phase,
            1,
        )

    PBX.write_text(text, encoding="utf-8")
    print(f"Updated {PBX.relative_to(ROOT)}")
    print("Next: place libluajit.a under scripting/lua/luajit/mac/ (see Doc/README.md)")


if __name__ == "__main__":
    main()
