# Combat System Documentation

## Overview

The combat system is built using a component-based architecture.

Characters are assembled from reusable gameplay components instead of storing
all logic inside one class.

The system supports:

- melee enemy attacks;
- ranged player attacks;
- health and armor;
- enemy AI;
- enemy spawning;
- death handling;
- projectile collisions;
- maze navigation.

---

# Character Hierarchy

## Character

Base gameplay class for all living entities.

Responsibilities:

- owns GameObject;
- stores common gameplay logic;
- used as parent class for Player and Enemy.

---

## Player

Derived from Character.

Responsibilities:

- movement;
- ranged attacks;
- receiving damage;
- interaction with enemies.

Uses:

- PlayerMovementComponent
- RangedAttackComponent
- StatsComponent
- DeathComponent

---

## Enemy

Derived from Character.

Base class for all enemy types.

Responsibilities:

- movement toward player;
- attacking player;
- enemy AI behavior.

Uses:

- PlayerSearchComponent
- AttackComponent
- DetectionTriggerComponent
- StatsComponent
- DeathComponent

Derived enemy types:

- Creeper
- Warrior

---

## Creeper

Derived from Enemy.

Fast aggressive enemy with lower health.

Features:

- high movement speed;
- short detection radius;
- aggressive chase behavior.

---

## Warrior

Derived from Enemy.

Balanced melee enemy.

Features:

- higher health;
- increased armor;
- slower movement speed.

---

# Components

## StatsComponent

Stores combat statistics.

Responsibilities:

- health;
- armor;
- damage processing;
- death state checks.

Main methods:

- `TakeDamage()`
- `IsDead()`
- `GetHealth()`

Damage is reduced using armor values.

---

## AttackComponent

Universal melee attack component.

Responsibilities:

- deal melee damage;
- attack nearby targets;
- process attack cooldowns.

Used by enemies.

---

## RangedAttackComponent

Handles player ranged attacks.

Responsibilities:

- create projectiles;
- launch projectiles;
- manage attack cooldown.

Attack flow:

1. Player presses LMB.
2. Projectile is created.
3. Projectile moves toward mouse position.
4. Projectile checks collisions.
5. Damage is applied on hit.
6. Projectile is destroyed.

---

## ProjectileComponent

Controls projectile behavior.

Responsibilities:

- projectile movement;
- collision detection;
- projectile lifetime;
- damage delivery.

Projectiles are destroyed when:

- hitting enemy;
- colliding with wall;
- lifetime expires.

Walls block projectiles, so enemies cannot be damaged through maze walls.

---

## DetectionTriggerComponent

Detects nearby objects using trigger radius.

Responsibilities:

- detect player entering range;
- detect player leaving range;
- notify enemy AI.

Used by enemies for player detection.

---

## PlayerSearchComponent

Controls enemy search and navigation.

Responsibilities:

- search player position;
- move toward player;
- navigate around maze walls.

Uses maze navigation system.

---

## DeathComponent

Handles object destruction.

Responsibilities:

- detect death state;
- mark object for destruction;
- print debug logs.

When health reaches zero:

- object becomes destroyed;
- GameWorld removes object safely later.

---

# Enemy Spawning

## EnemySpawner

Separate system responsible for enemy creation.

Responsibilities:

- spawn enemies;
- configure enemy count;
- configure enemy type;
- configure spawn distance.

Supports:

- Creeper spawning;
- Warrior spawning;
- random floor tile selection;
- safe spawn distance from player.

Example configurable settings:

- enemy count;
- minimum distance from player;
- enemy type.

---

# Maze Navigation

## MazeNavigation

Tile-based navigation system for enemies.

Responsibilities:

- avoid walls;
- move through maze corridors;
- navigate generated labyrinth.

Features:

- tile-based pathfinding;
- walkable tile checks;
- direction updates;
- maze-aware enemy movement.

---

# HUD

## GameHudComponent

Displays gameplay information.

Responsibilities:

- display player HP;
- display enemies remaining;
- display GAME OVER state;
- display YOU WIN state.

---

# Logging

The project actively uses Logger for debugging.

Examples:

- enemy detected player;
- enemy attacked player;
- projectile destroyed;
- player died;
- object destroyed;
- maze generated successfully.

---

# Combat Flow

## Player Attack Flow

1. Player presses LMB.
2. RangedAttackComponent creates projectile.
3. Projectile moves toward cursor.
4. Projectile checks collisions.
5. StatsComponent applies damage.
6. DeathComponent destroys enemy if HP <= 0.

---

## Enemy Attack Flow

1. Enemy detects player.
2. Enemy navigates through maze.
3. Enemy reaches attack distance.
4. AttackComponent deals damage.
5. Player receives damage.
6. Player dies if HP reaches zero.

---

# Architecture Notes

The project follows a modular architecture:

- gameplay logic is split into reusable components;
- enemies inherit from common base classes;
- systems are configurable through GameConfig;
- enemy spawning is fully data-driven;
- gameplay systems are separated from rendering logic.

This architecture makes the project easier to:

- extend;
- maintain;
- debug;
- scale with new enemy types and mechanics.