# Naruto Senki - V2

- [Naruto Senki - V2](#naruto-senki---v2)
  - [Introduction](#introduction)
    - [Changelog](#changelog)
    - [New features](#new-features)
      - [Game Mode](#game-mode)
    - [What have I done?](#what-have-i-done)
    - [What is the next?](#what-is-the-next)
  - [Know Issues](#know-issues)
  - [TODO](#todo)
  - [How to build?](#how-to-build)
    - [Adnroid](#adnroid)
    - [Linux](#linux)
    - [Windows](#windows)
  - [Structure](#structure)
  - [How to develop?](#how-to-develop)
  - [What is NarutoSenki: Re?](#what-is-narutosenki-re)

---

> For More details visit [game.naruto.re](https://game.naruto.re)
>
> Based on NarutoSenki source code

Get the latest developments👇
- Telegram: [t.me/narutosenki_re](https://t.me/narutosenki_re)
- Discord: [discord.gg/JGA6P8p](https://discord.gg/JGA6P8p)
- Youtube: [Re Offical Channel](https://www.youtube.com/channel/UCL9gDeedGZdf3hjRd-Zr7cg)

## Introduction

> Full desktop preview on [here](https://youtu.be/AaHkA1RmXxkg)

> `NOTE: This project will no longer develop after a few months`

### Changelog

- 2021-9-28
  - [UI] Added game mode selection panel
  - [UI] Added a return button to the select character panel on the desktop
  - [Added] Clone Mode ([Click me](#game-mode))
  - [Added] Random Deathmatch Mode ([Click me](#game-mode))
  - [Added] Click logo will enable custom select for all game modes (Except 1v1 and clone)
  - [Added] If no character is selected in any mode, it will be selected randomly
  - [Fixed] V2 version bugs (Lee, Kakuzu, Kisame)

- 2021-9-29
  - [Fixed] Audio playback error in debug mode on windows

### New features 

#### Game Mode

- Clone

> 

- Random Deathmatch

> 

### What have I done?

- **UPDATED**
  - Updated the project to cocos2d-x 2.2.6 & Visual Studio 2022 preview
  - Most dependent libraries have been upgraded to the latest version
  - Updated glfw2 to glfw3
  - [Adnroid] Support android 12.0 and rounded icons
  - [Adnroid] Support android arm64-v8a
  - [Linux] Use OpenAL instead of FMod
  - [Windows] Use OpenAL instead of native music player
  - [Assets] Use `ogg` format instead of `mp3` (Only support ogg)
  - Upgrade LuaJit to git master（Updated luajit android build script）
  - Added old characters Orochimaru/Pain/Kimimaro
- **FIXED**
  - Correct wrong words && Use english word instead of some chinese variables
  - Combined `NetworkLayer` and `SelectLayer` to `SelectLayer`
  - Separated a few of highly coupled code (All in `Core` folder)
  - Make some not core UI classes implemented by lua
  - [Config] Added some not existed sound files (`HokageMinato_xxx`, etc...)
  - [Gameplay] Fixed character spawn on the wrong direction when the character is right team `Akatsuki`
  - [Gameplay] Fixed character will flip when player only move up or down
  - [Gameplay] Fixed sometimes you can't move when you touched a tower (It still exist)
  - [Android] Fixed battle not fluent
  - [Windows] Fixed high CPU usage (It happened on low cocos2d-x version)
  - [UI] Extended map background
  - [UI] Fullscreen menu background
  - [UI] Make a new battle UI for Desktop platform
- **REMOVED**
  - All network codes & dependence (websocket)
  - [Config] Removed `Hidan` & `Kakuzu` & `Chiyo` hurt sound (It doesn't existed)
- **FEATURES**
  - [Linux & Windows] Half keyboard supported (Only working on battle scene)

### What is the next?

> I dont't want this project to take up more of my time.
>
> So I will back to the main work ---
> continue develop [NarutoSenki: Re](#what-is-narutosenki-re)
>
> I may continue to develop this project if I want.

## Know Issues

- Windows
  - You tell me !
- Linux
  - The `Text Component` manifested are incomplete.
- Android
  - Some sounds will not playing when the battle starts in the first few seconds.
  - Menu's notice clipper not working on android 10.0+. (But it works great on the other platforms)
  - Not be fluent on your android 10.0+ device.

## TODO

- Support shortcut switch from `windowed` to `fullscreen` and more
- Use `OpenAL` play music on `android` platform (May be fixed sounds not wordking)

---

## How to build?

### Adnroid

Open `project/NarutoSenki/proj.android` with Android Studio

Press `Ctrl + F9` to make project

### Linux

dependence:
  - Arch
    > `pacman -Syu freetype gcc glfw freealut pthread sqlite3 tolua++ gdb`

    or you can also use yay

    > `yay -Syu freetype gcc glfw freealut pthread sqlite3 tolua++ gdb`

``` bash
git clone https://github.com/real-re/NarutoSenki-V2
cd NarutoSenki-V2
./build.sh
```
or
``` bash
cd NarutoSenki-V2
cd tools/tolua++
make
cd ../../
make # DEBUG=1 # -j12
```

### Windows

- Open `cocos2d-win32.sln` with Visual Studio 2022

or

- Run `build.bat`

### macOS

Requires **Xcode** and **Python 3** (to run the Xcode patch utility).

From the repository root:

1. Build **LuaJIT** as a static library (outputs `scripting/lua/luajit/mac/libluajit.a`).
   Clone [LuaJIT](https://github.com/LuaJIT/LuaJIT) into `scripting/lua/luajit/LuaJIT`, then run:

```bash
cd scripting/lua/luajit
./build_mac_luajit.sh
```

2. Patch the Xcode project so it links Naruto Senki gameplay and Lua scripting sources:

```bash
python3 tools/patch_mac_xcode.py
```

3. Open `projects/NarutoSenki/proj.mac/hello.xcodeproj` in Xcode.

If the build fails because **`external/imgui/...` is missing**, the cocos2dx Mac project may still reference Dear ImGui (not shipped in this repo; ImGui is off in `EAGLView.mm`). Run once from the repo root:

```bash
python3 tools/strip_imgui_from_cocos2dx_mac_xcode.py
```

If Xcode reports **missing `external/chipmunk/...` or `CMakeLists.txt` under Chipmunk** (template points to vendored physics that is not in this tree), run once:

```bash
python3 tools/strip_chipmunk_from_hello_mac_xcode.py
```

4. Add **folder references** (blue folders) for `projects/NarutoSenki/Resources` and `projects/NarutoSenki/lua` to the application target so they are copied into `.app/Contents/Resources/` (needed for gameplay assets and Lua at runtime).

5. Optional: rename the Xcode target/product from `hello` to `Naruto Senki`; set **C++ language dialect** to C++17 or C++20 to match other desktop builds; set **macOS Deployment Target** to **11.0** or newer if Xcode warns (the project is updated to **11.0** in tree).

```bash
xcodebuild -project projects/NarutoSenki/proj.mac/hello.xcodeproj -scheme hello -configuration Debug
```

## Structure

```
root: projects/NarutoSenki

├───Classes
│   ├───Core
│   │   ├───Bullet
│   │   ├───Guardian
│   │   ├───Kuchiyose
│   │   ├───Scripts
│   │   ├───Shinobi
│   │   │   └───Bunshin
│   │   ├───Tower
│   │   └───Warrior
│   └───MyUtils
├───lua
│   ├───class
│   ├───core
│   ├───framework
│   │   └───cocos2dx
│   ├───ui
│   └───utils
├───proj.android-studio
├───proj.ios
├───proj.linux
├───proj.mac
├───proj.win32
├───Resources
│   ├───Audio
│   ├───Effects
│   ├───Element
│   ├───Fonts
│   └───Tiles
├───sprites
└───__BIN__
```

- Meaning
  - Classes : C++ source files
    - Core
      - Bullet
      - Guardian
      - Kuchiyose
      - Scripts
      - Shinobi
        - Bunshin
    - MyUtils
  - lua : Lua script files
  - proj.android-studio : Android project
  - proj.ios : IOS project (Not support)
  - proj.linux : Linux project
  - proj.mac : macOS (Xcode) — see **How to build › macOS**
  - proj.win32 : Windows project
  - Resources
  - sprites : Unused assets
  - \_\_BIN__ : Build output (Linux & Windows)

## How to develop?

- Tools
  - [TexturePacker](https://www.codeandweb.com/texturepacker)
  - [TextureUnpacker](https://github.com/real-re/TextureUnpacker)
    - Batch split textures that made by TexturePacker
  - [Tiled](https://www.mapeditor.org/)
    - Can create maps (Get more rules open `xxx.tmx` in `projects/NarutoSenki/Resources/Tiles/` with tiled)

## What is NarutoSenki: Re?

> Made with Unity3D and still in development naruto video game.
>
> For now, looks like it's just NarutoSenki developed with another game engine.

You can get all the notices at the above `telegram` or `discord`.

---

**<font color=#D32F2F>M</font><font color=#FF5722>a</font><font color=#FFEB3B>d</font><font color=#8BC34A>e</font> <font color=#00BCD4>b</font><font color=#448AFF>y</font> <font color=#673AB7>R</font><font color=#FF4081>e</font>**
