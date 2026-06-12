# Roguelike

A small C++/SFML roguelike built around a simple component-based architecture.

The project is intentionally kept easy to explain: game objects are composed from small components, gameplay settings live in `GameConfig`, and generated resources are loaded through the shared resource system.

## Features

- procedural maze generation;
- randomized floor and wall tile textures;
- enemy detection and maze navigation;
- melee and ranged combat;
- projectile collisions with enemies and walls;
- inventory, equipment, hotbar, and HUD UI;
- level progression and background music;
- logger system;
- combat and UI system documentation.

## Controls

- WASD - movement;
- Left Mouse Button - ranged attack;
- I - inventory;
- 1-6 - use hotbar slot;
- Space - start, restart, or continue to next level;
- Escape - pause menu.

## Architecture Overview

### Component-Based Objects

Game entities are built from focused components:

- `TransformComponent` stores position, rotation, and scale;
- `SpriteRendererComponent` draws textures;
- collider components provide physics bounds;
- `StatsComponent`, `AttackComponent`, and `DeathComponent` describe combat behavior;
- UI panels are separate `UIElement` classes.

This keeps classes small and avoids large inheritance chains. Inheritance is used mainly for broad game concepts such as `Character`, `Player`, `Enemy`, `Creeper`, and `Warrior`.

### Maze And Navigation

`MazeGenerator` creates a walkable grid and then builds `Floor` and `Wall` objects from it. `MazeNavigation` stores that grid and finds paths for enemies through walkable cells.

Wall visuals and wall collision are separated: `Wall` uses `SpriteRendererComponent` for the texture and `BoxColliderComponent` for stable tile collision.

### Resources

Gameplay resources are loaded in `GameResourceLoader`.

Tile variants are organized as:

- `Resources/Textures/Floors/Floor_1.png` ... `Floor_8.png`;
- `Resources/Textures/Walls/Wall_1.png` ... `Wall_8.png`.

The texture keys used by level generation are listed in `GameConfig`.

### Combat

Player attacks are projectile-based. Enemies use detection triggers, pathfinding, and melee attacks.

More details are documented in `docs/combat-system.md`.

### UI And Run State

The UI is split into HUD, inventory, equipment, hotbar, popup, and screen overlay panels.
Run state between levels is stored through `SaveSystem`, so inventory, equipment, hotbar, health,
armor, attack power, and movement speed can carry into the next generated level.

More details are documented in `docs/ui-system.md`.

## Build

Recommended environment:

- Visual Studio 2022;
- C++17;
- SFML 2.5.1.

Build the `Roguelike` project in `Debug|x64` or `Release|x64`.

The project copies `Roguelike/Resources` to the output folders after build, so textures, UI assets, fonts, and sounds are available when the game starts.

Convenience scripts from the repository root:

- `build.bat` - builds `Debug|x64`;
- `build-debug.bat` - builds `Debug|x64`;
- `build-release.bat` - builds `Release|x64`.

Pass `--no-pause` to any script when running from an existing terminal or CI-like shell.

## Tests

After building, run:

```powershell
.\x64\Debug\EngineTest.exe
.\x64\Release\EngineTest.exe
```

The current test suite covers vector math and typed `SaveSystem` behavior.

## Logs

Logs are written to:

`Roguelike/Saved/Logs/log.txt`

Build outputs, `dist/`, and runtime logs are generated artifacts and are ignored by git.
