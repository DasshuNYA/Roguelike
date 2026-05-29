# Architecture Overview

This document is a short map of the project for code review and defense.

## Layers

The project is split into three main parts:

- `Engine` contains reusable infrastructure: the game loop, world storage, game objects,
  components, physics, rendering, resources, audio, base UI classes, and math helpers.
- `Roguelike` contains game-specific behavior: characters, attacks, item pickup,
  inventory, level generation, enemy spawning, UI screens, and configuration.
- `EngineTest` contains GoogleTest coverage for engine-level code.

This split keeps the game logic from becoming tied to low-level engine details.

## Runtime Flow

`Engine::Run()` starts the active scene, then repeats this loop while the window is open:

1. process window events;
2. update game objects;
3. update physics with a fixed timestep;
4. render the world;
5. destroy objects that were marked for removal;
6. restart the scene if requested.

`GameWorld` owns active `GameObject` instances. A paused world updates only objects that
were explicitly marked as pause-ignored. The UI object uses this so menus and overlays
continue to work while gameplay is paused.

## Components

Each `GameObject` is built from small components:

- `TransformComponent` stores position, rotation, scale, and parent-child transforms;
- `SpriteRendererComponent` draws textured sprites;
- collider components provide collision bounds;
- `RigidbodyComponent` stores physics velocity;
- game components such as `StatsComponent`, `AttackComponent`, and `InventoryComponent`
  add gameplay behavior.

This makes entities composable: the player and enemies share engine components, but use
different game components and configuration.

## Resources

`ResourceSystem` stores shared textures, texture maps, and sound buffers by string keys.
`GameResourceLoader` is the game-specific loading point and maps those keys to files in
`Roguelike/Resources`.

The resource system logs missing assets and returns `nullptr` for invalid requests instead
of silently dereferencing missing data.

## Configuration

`GameConfig` contains the important gameplay constants:

- window and tile sizes;
- player/enemy stats;
- projectile values;
- spawn counts and distances;
- inventory dimensions;
- item definitions;
- background music tracks.

For defense, this is the best place to show that balance and content values are not hard
coded deep inside gameplay classes.

## Tests

`EngineTest` currently covers `Vector2D`, the small math type used by movement, physics,
and gameplay helpers. The test project is intentionally separate from `Roguelike` so engine
code can be verified without starting the full game.
