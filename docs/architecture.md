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

`GameWorld` owns active `GameObject` instances through `unique_ptr`, and each object owns
its components the same way. A paused world updates only objects that were explicitly
marked as pause-ignored. The UI object uses this so menus and overlays continue to work
while gameplay is paused.

## Components

Each `GameObject` is built from small components:

- `TransformComponent` stores an object's world position;
- `SpriteRendererComponent` draws textured sprites;
- collider components provide collision bounds;
- `RigidbodyComponent` stores physics velocity;
- game components such as `StatsComponent`, `AttackComponent`, and `InventoryComponent`
  add gameplay behavior.

`PlayerItemEffectsComponent` owns the gameplay consequences of consumables and equipment.
This keeps `GameUIComponent` focused on screen state and user interaction instead of changing
player stats directly.

This makes entities composable: the player and enemies share engine components, but use
different game components and configuration.

`Character::BuildCharacter` assembles the components shared by the player and every enemy.
`Player` and `Enemy` then add only their specific behavior. This removes duplicated setup while
keeping component ownership in `GameObject`.

## Resources

`ResourceSystem` stores shared textures and sound buffers by string keys.
`GameResourceLoader` is the game-specific loading point and maps those keys to files in
`Roguelike/Resources`.

Resources are stored by value, so their lifetime is automatic and no manual `new`/`delete`
is needed. Missing assets are logged and invalid requests return `nullptr`.

## Configuration

`GameConfig` contains the important gameplay values and entity definitions:

- window and tile sizes;
- player/enemy stats;
- projectile values;
- spawn counts and distances;
- inventory dimensions;
- item definitions;
- background music tracks.

`PlayerEntity` describes the player. The `EnemyTypes` table describes every regular enemy,
including its textures, stats, movement, detection, and spawn rules. `DeveloperLevel` and
`EnemySpawner` iterate this table, so adding a regular enemy does not require a new subclass,
enumeration value, factory branch, or level-specific spawn code.

Character settings are grouped into named `textures`, `stats`, and `movement` sections, while
enemy placement uses a separate `spawn` section. Values in the entity definitions are placed on
labelled lines so balance can be edited without memorizing a long positional parameter list.

For defense, this is the best place to show that balance and content values are not hard
coded deep inside gameplay classes.

## Tests

`EngineTest` covers `Vector2D` and typed `SaveSystem` behavior. The test project is
intentionally separate from `Roguelike` so engine code can be verified without starting
the full game.
