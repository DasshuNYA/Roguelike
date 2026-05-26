// @file GameConfig.h

#pragma once

#include "ItemData.h"

#include <array>

namespace Roguelike
{
namespace GameConfig
{
struct BackgroundTrackConfig
{
    const char* name;
    const char* path;
};

inline constexpr float MusicVolume = 25.f;
inline constexpr std::array<BackgroundTrackConfig, 4> BackgroundTracks = {
    {{"bg_level_1", "Resources/Sounds/background_1.ogg"},
     {"bg_level_2", "Resources/Sounds/background_2.ogg"},
     {"bg_level_3", "Resources/Sounds/background_3.ogg"},
     {"bg_level_4", "Resources/Sounds/background_4.ogg"}}};

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

inline constexpr int ItemSpawnCount = 18;
inline constexpr float ItemMinSpawnDistanceFromPlayer = 180.f;
inline constexpr float ItemPickupRadius = 28.f;
inline constexpr float ItemIconSize = 30.f;
inline constexpr int InventoryColumns = 4;
inline constexpr int InventoryRows = 2;
inline constexpr int InventoryPages = 4;
inline constexpr int InventorySlotCount = InventoryColumns * InventoryRows * InventoryPages;

inline constexpr std::array<ItemData, 6> Items = {
    {{"Armor", "A worn breastplate. Heavy, honest protection.", ItemTag::Equipment,
      EquipmentSlotType::Armor, 1, 135, 150, 170},
     {"Helmet", "A dented helmet that still knows its job.", ItemTag::Equipment,
      EquipmentSlotType::Head, 1, 185, 180, 150},
     {"Speed Potion", "A bright draught that makes every step lighter.", ItemTag::Consumable,
      EquipmentSlotType::None, 1, 80, 190, 235},
     {"Health Potion", "A warm red potion with a reassuring glow.", ItemTag::Consumable,
      EquipmentSlotType::None, 1, 205, 55, 75},
     {"Attack Potion", "A sharp bitter brew that wakes up your weapon hand.", ItemTag::Consumable,
      EquipmentSlotType::None, 1, 220, 105, 50},
     {"Boots", "Soft leather boots made for quick exits.", ItemTag::Equipment,
      EquipmentSlotType::Boots, 1, 150, 95, 55}}};

}  // namespace GameConfig
}  // namespace Roguelike
