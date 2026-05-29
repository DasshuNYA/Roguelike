# Roguelike

Small roguelike prototype built with a custom C++ component-based engine and SFML.

---

# Features

- Procedural maze generation
- Component-based architecture
- Character inheritance hierarchy
- Enemy AI
- Ranged combat system
- Projectile collisions with walls
- Configurable enemy spawning
- HUD system
- Logging system
- Modular gameplay systems

---

# Character Hierarchy

```text
Character
├── Player
└── Enemy
    ├── Creeper
    └── Warrior
```

---

# Main Systems

## Character System

Base Character class used for:

- Player
- Enemy

Enemy class is used as base class for:

- Creeper
- Warrior

---

## Combat System

The combat system supports:

- enemy melee attacks;
- ranged player attacks;
- projectiles;
- HP and armor;
- death handling.

Player uses ranged attacks with projectiles.

Projectiles:
- move toward enemies;
- collide with walls;
- disappear on hit;
- have configurable speed and lifetime.

---

## Enemy AI

Enemies:

- detect player in radius;
- navigate through maze;
- avoid walls;
- move toward player;
- attack player.

---

## Enemy Spawner

EnemySpawner is responsible for:

- enemy creation;
- configurable enemy count;
- configurable enemy types;
- safe spawn positions.

Spawner supports:
- Creeper spawning;
- Warrior spawning;
- random floor tile selection;
- minimum distance from player.

---

## HUD

HUD displays:

- player HP;
- enemy count;
- GAME OVER state;
- YOU WIN state.

---

## Logging

The project uses a logger system with:

- console logs;
- file logs;
- Saved/Logs directory.

---

# Technologies

- C++
- SFML
- Custom component-based engine
- clang-format

---

# Project Structure

```text
Engine/
├── Core/
├── Components/
├── Physics/
├── Rendering/
├── Resources/
└── Math/

Roguelike/
├── Characters/
├── Components/
├── Gameplay/
├── Config/
├── Resources/
└── Saved/
```

---

# Build

Requirements:

- Visual Studio 2022
- SFML 2.5.1
- Windows x64

---

# Controls

- WASD — movement
- Left Mouse Button — ranged attack

---

# Logs

Runtime logs are written to `Roguelike/Saved/Logs/log.txt`.
Console output shows the main `INFO`, `WARN`, and `ERROR` messages, while the file log also keeps `DEBUG` messages and source locations.

---

# Author

Educational project created for gameplay architecture and combat system practice.
