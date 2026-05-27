// @file GameResourceLoader.cpp

#include "pch.h"
#include "GameResourceLoader.h"

#include "GameConfig.h"
#include "ResourceSystem.h"

#include <string>

namespace Roguelike
{
namespace
{
void LoadNumberedTileTextures(const std::string& keyPrefix,
                              const std::string& folder,
                              const std::string& filePrefix,
                              int count)
{
    for (int i = 1; i <= count; ++i)
    {
        const std::string index = std::to_string(i);
        Engine::ResourceSystem::Instance()->LoadTexture(
            keyPrefix + "_" + index,
            "Resources/Textures/" + folder + "/" + filePrefix + "_" + index + ".png");
    }
}
}

void GameResourceLoader::Load()
{
    Engine::ResourceSystem::Instance()->LoadTexture("player", "Resources/Textures/Player.png");

    Engine::ResourceSystem::Instance()->LoadTexture("creeper", "Resources/Textures/Creeper.png");

    Engine::ResourceSystem::Instance()->LoadTexture("enemy", "Resources/Textures/Enemy.png");

    // Tile variants are grouped by folder and loaded by predictable numbered names.
    LoadNumberedTileTextures("wall", "Walls", "Wall",
                             static_cast<int>(GameConfig::WallTextureKeys.size()));
    LoadNumberedTileTextures("floor", "Floors", "Floor",
                             static_cast<int>(GameConfig::FloorTextureKeys.size()));

    // Compatibility fallback for older output folders that still contain Wall.png/Floor.png.
    Engine::ResourceSystem::Instance()->LoadTexture("wall_1", "Resources/Textures/Wall.png");
    Engine::ResourceSystem::Instance()->LoadTexture("floor_1", "Resources/Textures/Floor.png");

    Engine::ResourceSystem::Instance()->LoadTexture("ui_inventory_menu_top",
                                                    "Resources/UI/inventory_menu_top.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_inventory_menu_grid",
                                                    "Resources/UI/inventory_menu_grid.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture(
        "ui_inventory_menu_description", "Resources/UI/inventory_menu_description.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_slot_equipment",
                                                    "Resources/UI/slot_equipment.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_slot_weapon",
                                                    "Resources/UI/slot_weapon.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_slot_inventory",
                                                    "Resources/UI/slot_inventory.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_player_avatar",
                                                    "Resources/UI/player_avatar.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture(
        "ui_hud_health_bar_frame", "Resources/UI/hud_health_bar_frame.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_hud_health_bar_fill",
                                                    "Resources/UI/hud_health_bar_fill.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_player_armor",
                                                    "Resources/UI/player_armor.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_popup_message",
                                                    "Resources/UI/popup_message.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_hotbar_frame",
                                                    "Resources/UI/hotbar_frame.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_slot_hotbar",
                                                    "Resources/UI/slot_hotbar.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_item_armor",
                                                    "Resources/UI/item_armor.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_item_helmet",
                                                    "Resources/UI/item_helmet.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_item_boots",
                                                    "Resources/UI/item_boots.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture(
        "ui_item_speed_potion", "Resources/UI/item_speed_potion.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture(
        "ui_item_health_potion", "Resources/UI/item_health_potion.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture(
        "ui_item_attack_potion", "Resources/UI/item_attack_potion.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture(
        "ui_start_game_background", "Resources/UI/start_game_background.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture("ui_dead_background",
                                                    "Resources/UI/1dead_background.png", false);
    Engine::ResourceSystem::Instance()->LoadTexture(
        "ui_next_level_background", "Resources/UI/next_level_background.png", false);

    for (const GameConfig::BackgroundTrackConfig& track : GameConfig::BackgroundTracks)
    {
        Engine::ResourceSystem::Instance()->LoadSoundBuffer(track.name, track.path);
    }
}
}  // namespace Roguelike
