// @file GameConfig.h

#pragma once

namespace Roguelike
{
namespace GameConfig
{
inline constexpr float MusicVolume = 10.f;

inline constexpr float TileSize = 64.f;

inline constexpr int MazeWidth = 21;
inline constexpr int MazeHeight = 15;

inline constexpr int CharacterPixelSize = 48;
inline constexpr int TilePixelSize = 64;

inline constexpr float PlayerHealth = 100.f;
inline constexpr float PlayerArmor = 0.f;
inline constexpr float PlayerAttackPower = 25.f;
inline constexpr float PlayerStartTileX = 1.f;
inline constexpr float PlayerStartTileY = 1.f;

inline constexpr float ProjectileSpeed = 700.f;
inline constexpr float ProjectileLifeTime = 1.5f;
inline constexpr float ProjectileRadius = 8.f;

inline constexpr float CreeperHealth = 50.f;
inline constexpr float CreeperArmor = 0.f;
inline constexpr float CreeperAttackPower = 30.f;
inline constexpr float CreeperSpeed = 180.f;
inline constexpr float CreeperDetectionRadius = 250.f;

inline constexpr float WarriorHealth = 100.f;
inline constexpr float WarriorArmor = 5.f;
inline constexpr float WarriorAttackPower = 15.f;
inline constexpr float WarriorSpeed = 130.f;
inline constexpr float WarriorDetectionRadius = 250.f;

inline constexpr int CreeperSpawnCount = 1;
inline constexpr int WarriorSpawnCount = 4;
inline constexpr float EnemyMinSpawnDistanceFromPlayer = 300.f;

inline constexpr float HudPositionX = 20.f;
inline constexpr float HudPositionY = 20.f;
inline constexpr unsigned int HudFontSize = 24;

inline constexpr const char* HudFontPath = "Resources/Fonts/Roboto-Regular.ttf";
}  // namespace GameConfig
}  // namespace Roguelike