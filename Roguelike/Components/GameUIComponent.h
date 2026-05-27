// @file GameUIComponent.h

#pragma once

#include "Component.h"
#include "EquipmentPanel.h"
#include "GameObject.h"
#include "GameScreenOverlay.h"
#include "HUD.h"
#include "HotbarPanel.h"
#include "InventoryComponent.h"
#include "InventoryPanel.h"
#include "ItemDescriptionPanel.h"
#include "PopupMessage.h"
#include "UIItemView.h"
#include "UIManager.h"

#include <SFML/Graphics.hpp>

#include <optional>
#include <string>
#include <vector>

namespace Roguelike
{
class GameUIComponent : public Engine::Component
{
   public:
    explicit GameUIComponent(Engine::GameObject* gameObject);

    void SetPlayer(Engine::GameObject* player);
    void SetLevelObjective(const std::vector<Engine::GameObject*>& enemies, int level);

    void Update(float deltaTime) override;
    void Render() override;

   private:
    void CreateUI();
    void UpdateHUD();
    void UpdateInventory();
    void UpdateLevelObjective(float deltaTime);
    void AutoPlaceNewHotbarItems(const std::vector<ItemStack>& items);
    int GetKnownItemCount(const ItemData* itemData) const;
    void HandleInput(sf::RenderWindow& window);
    void HandleMainMenuInput();
    void HandlePauseInput();
    void HandleInventoryInput(sf::RenderWindow& window);
    void HandleHotbarInput();
    void HandleDeathState();
    void HandleGameOverInput();
    void HandleLevelCompleteInput();
    void ToggleInventory();
    void CloseInventory();
    void ClearSelectedItem();
    void SelectItem(const UIItemView& item);
    bool TryPlaceSelectedItem(sf::Vector2f mousePosition);
    void ShowPopupMessage(const std::string& message, float duration = 1.5f);
    void DrawDraggedItem(sf::RenderWindow& window);

   private:
    Engine::GameObject* playerObject = nullptr;
    InventoryComponent* playerInventory = nullptr;
    std::vector<Engine::GameObject*> objectiveEnemies;
    int levelNumber = 1;
    int totalEnemyCount = 0;
    int aliveEnemyCount = 0;
    bool isLevelComplete = false;

    Engine::UIManager uiManager;

    sf::Font font;
    sf::Font titleFont;

    HUD* hud = nullptr;
    InventoryPanel* inventory = nullptr;
    EquipmentPanel* equipment = nullptr;
    HotbarPanel* hotbar = nullptr;
    ItemDescriptionPanel* descriptionPanel = nullptr;
    PopupMessage* popup = nullptr;
    GameScreenOverlay* overlay = nullptr;

    std::optional<UIItemView> draggedItem;
    std::vector<ItemStack> knownInventoryItems;

    bool isMainMenuOpen = true;
    bool isPauseOpen = false;
    bool isGameOver = false;

    bool wasStartPressed = false;
    bool wasRestartPressed = false;
    bool wasInventoryPressed = false;
    bool wasPausePressed = false;
    bool wasLeftMousePressed = false;
    bool wasRightMousePressed = false;
};
}  // namespace Roguelike
