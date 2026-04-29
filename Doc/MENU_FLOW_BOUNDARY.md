# Menu/UI Source-of-Truth Boundary

This project keeps menu presentation split between Lua and C++. To reduce drift:

- Lua is the source-of-truth for scene transitions (`enterSelectLayer`, `onGameOver`, `backToStartMenu`).
- C++ is responsible for input/widget state and game-mode selection payload.

## Callback Map

- `GameModeLayer` -> `enterSelectLayer(mode, enableCustomSelect)`
- `GameLayer` -> `onGameOver()`
- `SelectLayer` -> `backToStartMenu()`
- `CreditsLayer` -> `CreditsLayer_BackToStartMenu()`

All callback keys are centralized in `Classes/Constants/UiFlowKeys.hpp`.

## Rule

When adding a new menu callback:
1. Add key in `UiFlowKeys.hpp`.
2. Implement Lua global function in `lua/ui/StartMenu.lua` (or related scene file).
3. Call using bridge helper from C++, avoid hardcoded string literals.
