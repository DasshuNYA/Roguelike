# Roguelike

Small roguelike project made with C++ and SFML.

The project uses a custom component-based architecture.

---

## Features

- procedural maze generation;
- enemy AI;
- ranged combat;
- projectile system;
- enemy spawning;
- HUD;
- logger system;
- combat documentation;
- clang-format support.

---

## Character Hierarchy

Character
- Player
- Enemy
  - Creeper
  - Warrior

---

## Controls

- WASD — movement
- Left Mouse Button — ranged attack

---

## Main Systems

### Combat System

Player uses projectile attacks.

Projectiles:
- move toward enemies;
- collide with walls;
- deal damage;
- disappear after hit.

### Enemy AI

Enemies:
- detect player;
- move through maze;
- avoid walls;
- attack player.

### EnemySpawner

Spawner supports:
- enemy type setup;
- enemy count setup;
- spawn distance setup.

---

## Technologies

- C++
- SFML
- Visual Studio 2022

---

## Logs

Logs are stored in:
Saved/Logs