// @file GameUIComponent.h

#pragma once

#include "Component.h"
#include "ItemData.h"
#include "UIConfig.h"
#include "UIItemView.h"
#include "UIManager.h"

#include <SFML/Graphics.hpp>

#include <optional>
#include <string>
#include <vector>

namespace Engine
{
class GameObject;
class StatsComponent;
}  // namespace Engine

namespace Roguelike
{
class EquipmentPanel;
class GameScreenOverlay;
class HotbarPanel;
class HUD;
class InventoryComponent;
class InventoryPanel;
class ItemDescriptionPanel;
class PlayerItemEffectsComponent;
class PlayerMovementComponent;
class PopupMessage;

class GameUIComponent : public Engine::Component
{
   private:
    enum class ScreenState
    {
        MainMenu,
        Playing,
        Paused,
        GameOver,
        LevelComplete
    };

   public:
    explicit GameUIComponent(Engine::GameObject* gameObject);
    ~GameUIComponent() override;

    void SetPlayer(Engine::GameObject* player);
    void SetLevelObjective(const std::vector<Engine::GameObject*>& enemies, int level);

    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void Render() override;

   private:
    // UI creation and frame refresh.
    void CreateUI();
    void SetScreenState(ScreenState newState);
    bool IsGameplayScreen() const;
    void SyncWorldPause();
    void UpdateHUD();
    void UpdateHUDStats(float health, float armor);
    void UpdateInventory();
    void UpdateLevelObjective(float deltaTime);

    // Inventory/hotbar synchronization.
    void AutoPlaceNewHotbarItems(const std::vector<ItemStack>& items);
    int GetKnownItemCount(const ItemData* itemData) const;

    // Modal input flow. Game over, level complete, main menu, and pause consume input first.
    void HandleKeyPressed(sf::Keyboard::Key key);
    void HandleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton,
                                  sf::RenderWindow& window);
    void HandleStartPressed();
    void HandlePausePressed();
    void HandleInventoryTogglePressed();
    void HandleInventoryClick(sf::Vector2f mousePosition);
    void HandleInventoryCancelPressed();
    void HandleHotbarKey(sf::Keyboard::Key key);
    void HandleDeathState();
    void HandleGameOverRestartPressed();
    void HandleLevelCompleteNextPressed();

    // Run persistence between levels.
    void RestorePlayerRunState();
    void SavePlayerRunState();

    // Drag-and-drop selection helpers shared by inventory, equipment, hotbar, and description.
    void ToggleInventory();
    void CloseInventory();
    void ClearSelectedItem();
    void SelectItem(const UIItemView& item);
    bool TryPlaceSelectedItem(sf::Vector2f mousePosition);
    bool TryEquipSelectedItem(sf::Vector2f mousePosition);
    bool TryAssignSelectedItemToHotbar(sf::Vector2f mousePosition);
    void ShowPopupMessage(const std::string& message,
                          float duration = UIConfig::Popup::QuickDuration);
    void DrawDraggedItem(sf::RenderWindow& window);

   private:
    Engine::GameObject* playerObject = nullptr;
    InventoryComponent* playerInventory = nullptr;
    PlayerItemEffectsComponent* playerItemEffects = nullptr;
    PlayerMovementComponent* playerMovement = nullptr;
    Engine::StatsComponent* playerStats = nullptr;

    // Objective state: enemies are tracked as alive/total, HUD converts to killed/total.
    std::vector<Engine::GameObject*> objectiveEnemies;
    int levelNumber = 1;
    int totalEnemyCount = 0;
    int aliveEnemyCount = 0;
    ScreenState screenState = ScreenState::MainMenu;

    Engine::UIManager uiManager;

    // Fonts are loaded once and passed by reference into every panel.
    sf::Font font;
    sf::Font titleFont;

    HUD* hud = nullptr;
    InventoryPanel* inventory = nullptr;
    EquipmentPanel* equipment = nullptr;
    HotbarPanel* hotbar = nullptr;
    ItemDescriptionPanel* descriptionPanel = nullptr;
    PopupMessage* popup = nullptr;
    GameScreenOverlay* overlay = nullptr;

    // Currently dragged inventory item and snapshot used for hotbar auto-placement.
    std::optional<UIItemView> draggedItem;
    std::vector<ItemStack> knownInventoryItems;
};
}  // namespace Roguelike
