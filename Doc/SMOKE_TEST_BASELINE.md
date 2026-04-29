# NarutoSenki Smoke Test Baseline

This checklist freezes core gameplay behavior before major refactors.

## Scope

- Desktop builds (Mac/Linux/Windows), prioritize currently active platform first.
- Main flow: menu -> mode select -> battle -> game over -> back to menu.

## Test Matrix

### A. Boot and Menu

- Launch game and verify no Lua bootstrap error.
- Verify Start Menu renders correctly.
- Verify keyboard navigation still responds on desktop.

### B. Mode Selection

- Open game mode panel and switch modes:
  - `Classic`
  - `OneVsOne`
  - `RandomDeathmatch`
- Enter select scene from mode panel and ensure scene transition is smooth.

### C. Battle Start

- Start battle and verify:
  - HUD appears.
  - opening banner/sound plays once.
  - player can move after opening animation.

### D. Runtime Gameplay

- Verify `updateGameTime` clock increments each second.
- Verify background music check continues during battle.
- Spawn check:
  - towers exist and have HP bars.
  - flogs spawn when mode allows.
  - skill/effect sprites render and disappear correctly.

### E. End Flow

- Trigger win and lose game-over paths.
- Verify callback to menu scene and no stuck input state.

## Regression Notes Template

- Platform:
- Commit/branch:
- Mode tested:
- Result:
- Logs:
- Follow-up issue:
