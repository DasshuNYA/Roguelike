# Combat System Documentation

## What does the combat system include?

- StatsComponent - stores character stats such as health and armor.
- AttackComponent - handles melee attacks.
- RangedAttackComponent - handles player ranged attacks.
- ProjectileComponent - controls projectile movement and collisions.
- DetectionTriggerComponent - detects player in enemy radius.
- PlayerSearchComponent - controls enemy movement and chasing.
- EnemySpawner - responsible for enemy spawning.

---

## Main Features

### Damage System

AttackComponent and RangedAttackComponent allow characters to deal damage.
Enemies use melee attacks.
Player uses ranged projectile attacks.

When projectile collides with enemy:
- enemy receives damage;
- projectile gets destroyed.

Walls block projectiles.


---

## Health System

StatsComponent stores:
- health;
- armor;
- attack power.

StatsComponent contains methods for changing stats:
- SetStats(float health, float armor) - sets character stats.
- TakeDamage(float damage) - applies damage to character health and reduces damage using armor value.
- GetHealth() - returns current health.
- IsDead() - checks if character is dead.

When health reaches zero:
- DeathComponent destroys the object;
- GameWorld removes the object later.

---

## Character Hierarchy

Character
- Player
- Enemy
  - Creeper
  - Warrior

Character is the base class for all characters.
Enemy is the base class for enemy types.

---

## Player

Player uses:
- PlayerMovementComponent;
- RangedAttackComponent;
- StatsComponent;
- DeathComponent.

Player can:
- move;
- attack;
- receive damage;
- die.

---

## Enemy

Enemy uses:
- AttackComponent;
- DetectionTriggerComponent;
- PlayerSearchComponent;
- StatsComponent;
- DeathComponent.

Enemy can:
- detect player;
- move through maze;
- avoid walls;
- attack player.

---

### Enemy Creation

To create enemy object:

1. Create new class inherited from Enemy.

Example:
- Zombie;
- Skeleton;
- Mage;
- Boss.

2. Add required components:
- SpriteRendererComponent;
- SpriteColliderComponent;
- RigidbodyComponent;
- AttackComponent;
- DetectionTriggerComponent;
- PlayerSearchComponent;
- StatsComponent;
- DeathComponent.

3. Configure enemy parameters in GameConfig:
- health;
- armor;
- attack power;
- movement speed;
- detection radius;
- spawn count.

4. Set enemy texture in SpriteRendererComponent.

5. Add new enemy type into EnemySpawner.

---

## Projectile System

ProjectileComponent controls projectile behavior.

Main responsibilities:
- projectile movement;
- wall collision;
- enemy collision;
- projectile lifetime.

Main methods:
- SetDirection() - sets projectile movement direction.
- SetDamage() - sets projectile damage.
- SetSpeed() - sets projectile speed.
- SetLifeTime() - sets projectile lifetime.
- Update() - updates projectile movement and collisions.

When player presses Left Mouse Button:
1. projectile is created;
2. projectile moves toward target;
3. projectile checks collisions;
4. enemy receives damage;
5. projectile is destroyed.

Projectile also gets destroyed:
- after wall collision;
- after lifetime timeout.

---

## EnemySpawner

EnemySpawner is responsible for enemy creation.

Spawner supports:
- enemy type setup;
- enemy count setup;
- minimum spawn distance setup.

Currently used for:
- Creeper;
- Warrior.

Main methods:
- Spawn() - creates enemies on random floor positions.
- CreateEnemy() - creates enemy of selected type.
- IsPositionFarEnoughFromPlayer() - checks safe spawn distance.
- IsPositionAlreadyUsed() - prevents enemy overlap.
- Enemies spawn after maze generation.

---

## Maze Navigation

Enemies use MazeNavigation to move through the generated maze.

MazeGenerator creates a walkable grid after generation:
- true - floor tile;
- false - wall tile.

MazeNavigation uses this grid to find a path from enemy to player.
PlayerSearchComponent requests this path and moves enemy from one point to another.
This prevents enemies from walking through walls and allows them to chase player through maze corridors.

---
