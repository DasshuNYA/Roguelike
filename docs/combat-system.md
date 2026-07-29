# Combat System Documentation

This document describes the gameplay systems that are involved in combat, enemy detection, and enemy movement.

## Main Components

- `StatsComponent` stores health, armor, and attack power.
- `AttackComponent` handles melee attacks.
- `RangedAttackComponent` creates player projectiles.
- `ProjectileComponent` controls projectile movement, lifetime, and hit checks.
- `DetectionTriggerComponent` detects when the player enters or leaves enemy vision radius.
- `PlayerSearchComponent` moves enemies toward the detected player.
- `EnemySpawner` creates enemies on safe floor positions.
- `MazeNavigation` provides paths through generated maze corridors.

## Damage System

`AttackComponent` and `RangedAttackComponent` both use the stats system to apply damage.

Enemies use melee attacks when they are close enough to the player. The player uses ranged projectile attacks.

When a projectile hits an enemy:

- the enemy receives damage;
- the projectile is destroyed.

When a projectile hits a wall:

- the projectile is destroyed;
- the wall is not damaged.

## Health System

`StatsComponent` stores:

- health;
- armor;
- attack power.

Important methods:

- `SetStats(float health, float armor)` sets current health and armor values;
- `SetAttackPower(float attackPower)` sets attack damage;
- `TakeDamage(float damage)` applies armor reduction and changes health;
- `AddStatsChangedListener(...)` lets UI or gameplay helpers react when health or armor changes;
- `GetHealth()` returns current health;
- `IsDead()` checks whether health reached zero.

HUD health and armor are event-driven. `GameUIComponent` subscribes to the player's
`StatsComponent`, so damage immediately updates the HUD without polling player stats every
frame.

When health reaches zero, `DeathComponent` marks the object for destruction and `GameWorld` removes it later.

## Character Structure

The character hierarchy is intentionally small:

- `Character`
- `Player`
- `Enemy`
- `Creeper`
- `Warrior`

Most behavior comes from components, not from deep inheritance. This keeps enemy types easy to extend and explain.

## Player

The player uses:

- `PlayerMovementComponent`;
- `SpriteRendererComponent`;
- `SpriteColliderComponent`;
- `RigidbodyComponent`;
- `InventoryComponent`;
- `RangedAttackComponent`;
- `StatsComponent`;
- `DeathComponent`.

The player can move, shoot, collect items, receive damage, and die.

## Enemy

Enemies use:

- `SpriteRendererComponent`;
- `SpriteColliderComponent`;
- `RigidbodyComponent`;
- `AttackComponent`;
- `DetectionTriggerComponent`;
- `PlayerSearchComponent`;
- `StatsComponent`;
- `DeathComponent`.

Enemies can detect the player, request a path through the maze, move along that path, and attack in melee range.

## Detection Triggers

`DetectionTriggerComponent` is a collider marked as a trigger.

The physics system stores active trigger pairs, so `OnTriggerEnter` is fired once when two colliders start touching and `OnTriggerExit` is fired when they separate. This prevents repeated detection events every frame and clears stale trigger pairs when colliders are removed.

Enemy detection uses this flow:

1. Player enters enemy trigger radius.
2. `DetectionTriggerComponent` notifies `PlayerSearchComponent`.
3. `PlayerSearchComponent` starts requesting paths to the player.
4. Player leaves the trigger radius.
5. Enemy clears its current path and stops chasing.

## Maze Navigation

`MazeGenerator` creates a boolean walkable grid after generating the maze:

- `true` means floor/walkable tile;
- `false` means wall/blocked tile.

`MazeNavigation` stores this grid and uses A* search with four-direction movement. Four-direction movement matches the maze corridors and avoids diagonal wall clipping.

If an actor is slightly off-grid after physics movement, navigation tries to snap the start or target to the nearest walkable cell. This makes enemy movement more stable around walls and corners.

## Projectile System

`ProjectileComponent` controls:

- movement direction;
- speed;
- damage;
- collision checks against enemies;
- collision checks against walls;
- lifetime timeout.

When the player presses Left Mouse Button:

1. `RangedAttackComponent` creates a projectile object.
2. The projectile receives direction, damage, speed, radius, lifetime, targets, and obstacles.
3. The projectile moves every frame.
4. It is destroyed after hitting an enemy, hitting a wall, or reaching its lifetime limit.

## Wall Collision

Walls use two separate responsibilities:

- `SpriteRendererComponent` draws the selected wall texture;
- `BoxColliderComponent` provides stable tile collision.

This is important because wall textures can now have variants. Collision should not depend on the exact source image size or on whether a specific texture variant loaded correctly.

## EnemySpawner

`EnemySpawner` creates enemies after maze generation.

Spawner responsibilities:

- choose enemy types;
- create the requested amount of enemies;
- use floor positions from the generated maze;
- keep enemies away from the player spawn;
- avoid placing several enemies, including enemies of different types, on the same position.

Currently used enemy types:

- `Creeper`;
- `Warrior`.

To add a new enemy type:

1. Create a class derived from `Enemy`.
2. Add configuration values in `GameConfig`.
3. Load the enemy texture in `GameResourceLoader`.
4. Add the type to `EnemySpawner`.
5. Reuse the same component setup unless the enemy needs special behavior.
