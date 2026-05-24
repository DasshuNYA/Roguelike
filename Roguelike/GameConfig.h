// @file GameConfig.h

#pragma once

namespace Roguelike
{
namespace GameConfig
{
inline constexpr float PlayerHealth = 100.f;
inline constexpr float PlayerArmor = 0.f;
inline constexpr float PlayerAttackPower = 25.f;

inline constexpr float ProjectileSpeed = 700.f;
inline constexpr float ProjectileLifeTime = 1.5f;
inline constexpr float ProjectileRadius = 8.f;

inline constexpr float CreeperHealth = 50.f;
inline constexpr float CreeperArmor = 0.f;
inline constexpr float CreeperAttackPower = 30.f;
inline constexpr float CreeperSpeed = 180.f;
inline constexpr float CreeperDetectionRadius = 180.f;

inline constexpr float WarriorHealth = 100.f;
inline constexpr float WarriorArmor = 5.f;
inline constexpr float WarriorAttackPower = 15.f;
inline constexpr float WarriorSpeed = 130.f;
inline constexpr float WarriorDetectionRadius = 220.f;

inline constexpr int CreeperSpawnCount = 1;
inline constexpr int WarriorSpawnCount = 4;
inline constexpr float EnemyMinSpawnDistanceFromPlayer = 300.f;

inline constexpr int MazeWidth = 21;
inline constexpr int MazeHeight = 15;
}  // namespace GameConfig
}  // namespace Roguelike