\# Combat System Documentation



\## Overview



The combat system in the project is built using a component-based architecture.



Characters use different gameplay components:

\- StatsComponent

\- AttackComponent

\- DeathComponent

\- PlayerAttackComponent

\- PlayerSearchComponent



The system supports:

\- player attacks;

\- enemy attacks;

\- health and armor;

\- death handling;

\- enemy spawning;

\- target searching.



\---



\# Character Hierarchy



\## Character



Base gameplay class for all living entities.



Responsibilities:

\- owns GameObject;

\- stores common gameplay logic;

\- used as parent class for Player and Enemy.



\---



\## Player



Derived from Character.



Responsibilities:

\- player movement;

\- searching enemies nearby;

\- attacking enemies;

\- receiving damage.



Uses:

\- PlayerMovementComponent

\- PlayerAttackComponent

\- PlayerSearchComponent

\- StatsComponent

\- DeathComponent



\---



\## Enemy



Derived from Character.



Responsibilities:

\- detecting player;

\- moving to player;

\- attacking player.



Uses:

\- AttackComponent

\- DetectionTriggerComponent

\- StatsComponent

\- DeathComponent



\---



\## Creeper



Derived from Enemy.



Special enemy type with:

\- custom stats;

\- custom detection radius;

\- aggressive behavior.



\---



\# Components



\## StatsComponent



Stores combat statistics.



Responsibilities:

\- health;

\- armor;

\- attack power;

\- damage calculation.



Main methods:

\- TakeDamage()

\- IsDead()

\- GetHealth()



Damage is reduced by armor value.



\---



\## AttackComponent



Universal attack logic.



Responsibilities:

\- deal damage;

\- attack target;

\- check cooldowns.



Used by enemies.



\---



\## PlayerAttackComponent



Handles player attacks.



Responsibilities:

\- process left mouse button input;

\- attack nearby enemies;

\- work with targets from PlayerSearchComponent.



\---



\## PlayerSearchComponent



Searches nearby enemies.



Responsibilities:

\- detect enemies in radius;

\- maintain target list;

\- provide nearest enemy.



Used by PlayerAttackComponent.



\---



\## DeathComponent



Handles death state.



Responsibilities:

\- detect death;

\- destroy GameObject;

\- print debug logs.



When health reaches zero:

\- object becomes destroyed;

\- GameWorld removes object later.



\---



\# Enemy Spawning



\## EnemySpawner



Separate system responsible for enemy creation.



Responsibilities:

\- spawn enemies;

\- configure enemy count;

\- configure enemy types;

\- configure spawn distance.



Supports:

\- Creeper spawning;

\- random floor tile selection;

\- safe spawn distance from player.



Example settings:

\- enemy count;

\- minimum distance from player;

\- enemy type.



\---



\# Combat Flow



\## Player Attack Flow



1\. Player presses LMB.

2\. PlayerAttackComponent requests targets.

3\. PlayerSearchComponent returns nearby enemies.

4\. AttackComponent deals damage.

5\. StatsComponent reduces HP.

6\. DeathComponent destroys enemy if HP <= 0.



\---



\## Enemy Attack Flow



1\. Enemy detects player.

2\. Enemy moves toward player.

3\. Enemy attacks player.

4\. Player receives damage.

5\. Player dies when HP reaches zero.



\---



\# Maze Navigation



Enemies use maze navigation.



Responsibilities:

\- avoid walls;

\- move through corridors;

\- follow generated maze paths.



Implemented using:

\- tile-based navigation;

\- floor position checks;

\- path direction updates.



\---



\# HUD



\## GameHudComponent



Displays gameplay information.



Responsibilities:

\- display player HP;

\- display enemy count;

\- show GAME OVER;

\- show YOU WIN.



\---



\# Logging



The system actively uses Logger.



Examples:

\- enemy detected player;

\- enemy attacked player;

\- player died;

\- object destroyed.



