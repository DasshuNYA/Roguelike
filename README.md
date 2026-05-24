# Roguelike

A small component-based roguelike prototype written in C++ using SFML.

The project was created as part of a game development learning course and demonstrates:
- component architecture;
- procedural maze generation;
- player and enemy combat;
- enemy AI;
- scene and object systems;
- resource management;
- HUD rendering;
- pathfinding and navigation.

---

# Features

## Gameplay
- Procedural maze generation
- Player movement
- Enemy spawning
- Enemy detection system
- Melee combat
- Health and armor system
- Enemy AI navigation
- Victory and death states

---

# Architecture

The project uses:
- component-based architecture;
- inheritance for gameplay entities;
- scene-based world management.

---

# Main Systems

## Character System
Base Character class used for:
- Player
- Enemy
- Creeper

---

## Combat System
Includes:
- StatsComponent
- AttackComponent
- PlayerAttackComponent
- DeathComponent

Supports:
- damage;
- armor;
- death handling;
- attack cooldowns.

Detailed documentation:
- `docs/combat-system.md`

---

## Enemy Spawner
EnemySpawner is responsible for:
- enemy creation;
- configurable enemy count;
- configurable spawn rules;
- safe spawn distance from player.

---

## Maze Generation
MazeGenerator creates:
- floor tiles;
- walls;
- navigation layout.

Enemies use maze navigation to avoid walls and move through corridors.

---

# Technologies

- C++
- SFML
- Visual Studio 2022

---

# Build

## Requirements
- Visual Studio 2022
- SFML
- Windows x64

---

## Build Steps

1. Open `Game.sln`
2. Select:
   - Debug x64
   - or Release x64
3. Build solution

---

# Code Style

The project uses `clang-format`.

Formatting configuration:
- Allman braces
- 4 spaces indentation
- 80 column limit

---

# Project Structure

```text
Engine/         -> engine systems
Roguelike/      -> gameplay code
Resources/      -> textures, fonts, sounds
docs/           -> documentation