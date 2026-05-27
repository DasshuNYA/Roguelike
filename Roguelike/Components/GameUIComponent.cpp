// @file GameUIComponent.cpp

#include "pch.h"
#include "GameUIComponent.h"

#include "Engine.h"
#include "GameNotifications.h"
#include "GameWorld.h"
#include "LevelProgress.h"
#include "RenderSystem.h"
#include "StatsComponent.h"

namespace Roguelike
{
GameUIComponent::GameUIComponent(Engine::GameObject* gameObject) : Component(gameObject)
{
    font.loadFromFile("Resources/Fonts/Roboto-Regular.ttf");
    CreateUI();

    Engine::GameWorld::Instance()->SetPaused(true);
}

void GameUIComponent::SetPlayer(Engine::GameObject* player)
{
    playerObject = player;
    playerInventory =
        playerObject != nullptr ? playerObject->GetComponent<InventoryComponent>() : nullptr;
}

void GameUIComponent::SetLevelObjective(const std::vector<Engine::GameObject*>& enemies, int level)
{
    objectiveEnemies = enemies;
    levelNumber = level;
    totalEnemyCount = static_cast<int>(objectiveEnemies.size());
    aliveEnemyCount = totalEnemyCount;
    isLevelComplete = false;

    if (levelNumber > 1)
    {
        isMainMenuOpen = false;

        if (overlay != nullptr)
        {
            overlay->HideOverlay();
        }

        Engine::GameWorld::Instance()->SetPaused(false);
        ShowPopupMessage("Level " + std::to_string(levelNumber), 2.0f);
    }
}

void GameUIComponent::Update(float deltaTime)
{
    sf::RenderWindow& window = Engine::RenderSystem::Instance()->GetMainWindow();

    UpdateLevelObjective(deltaTime);
    HandleInput(window);
    UpdateHUD();
    UpdateInventory();

    std::optional<std::string> notification = GameNotifications::Consume();
    if (notification.has_value())
    {
        ShowPopupMessage(notification.value());
    }

    uiManager.Update(deltaTime);
}

void GameUIComponent::Render()
{
    sf::RenderWindow& window = Engine::RenderSystem::Instance()->GetMainWindow();

    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    uiManager.Draw(window);
    DrawDraggedItem(window);

    window.setView(oldView);
}

void GameUIComponent::CreateUI()
{
    hud = &uiManager.CreateElement<HUD>(font);
    hotbar = &uiManager.CreateElement<HotbarPanel>(font);
    inventory = &uiManager.CreateElement<InventoryPanel>(font);
    equipment = &uiManager.CreateElement<EquipmentPanel>(font);
    descriptionPanel = &uiManager.CreateElement<ItemDescriptionPanel>(font);
    popup = &uiManager.CreateElement<PopupMessage>(font);
    overlay = &uiManager.CreateElement<GameScreenOverlay>(font);

    overlay->ShowMainMenu();
}

void GameUIComponent::UpdateHUD()
{
    if (hud == nullptr)
    {
        return;
    }

    hud->SetObjective(levelNumber, aliveEnemyCount, totalEnemyCount);

    if (playerObject == nullptr)
    {
        return;
    }

    Engine::StatsComponent* stats = playerObject->GetComponent<Engine::StatsComponent>();

    if (stats == nullptr)
    {
        return;
    }

    hud->SetStats(stats->GetHealth(), 100.0f, stats->GetArmor(), 100.0f);
}

void GameUIComponent::UpdateLevelObjective(float deltaTime)
{
    if (isMainMenuOpen || isPauseOpen || isGameOver || isLevelComplete)
    {
        return;
    }

    aliveEnemyCount = 0;

    for (Engine::GameObject* enemy : objectiveEnemies)
    {
        if (Engine::GameWorld::Instance()->IsGameObjectAlive(enemy))
        {
            aliveEnemyCount++;
        }
    }

    if (aliveEnemyCount > 0 || totalEnemyCount <= 0)
    {
        return;
    }

    isLevelComplete = true;
    wasRestartPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    CloseInventory();

    if (overlay != nullptr)
    {
        overlay->ShowLevelCleared(levelNumber + 1);
    }

    Engine::GameWorld::Instance()->SetPaused(true);
}

void GameUIComponent::UpdateInventory()
{
    if (inventory == nullptr || playerInventory == nullptr)
    {
        return;
    }

    const std::vector<ItemStack>& items = playerInventory->GetItems();
    inventory->SetItems(items);
    AutoPlaceNewHotbarItems(items);
}

void GameUIComponent::AutoPlaceNewHotbarItems(const std::vector<ItemStack>& items)
{
    if (hotbar == nullptr)
    {
        knownInventoryItems = items;
        return;
    }

    for (const ItemStack& item : items)
    {
        if (!CanUseOnHotbar(item) || item.count <= GetKnownItemCount(item.data))
        {
            continue;
        }

        hotbar->TryAutoPlaceItem(UIItemView::FromStack(item));
    }

    knownInventoryItems = items;
}

int GameUIComponent::GetKnownItemCount(const ItemData* itemData) const
{
    for (const ItemStack& item : knownInventoryItems)
    {
        if (item.data == itemData)
        {
            return item.count;
        }
    }

    return 0;
}

void GameUIComponent::HandleInput(sf::RenderWindow& window)
{
    HandleDeathState();

    if (isGameOver)
    {
        HandleGameOverInput();
        return;
    }

    if (isLevelComplete)
    {
        HandleLevelCompleteInput();
        return;
    }

    if (isMainMenuOpen)
    {
        HandleMainMenuInput();
        return;
    }

    HandlePauseInput();
    HandleInventoryInput(window);
    HandleHotbarInput();
}

void GameUIComponent::HandleMainMenuInput()
{
    bool isStartPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    if (isStartPressed && !wasStartPressed)
    {
        isMainMenuOpen = false;

        if (overlay != nullptr)
        {
            overlay->HideOverlay();
        }

        Engine::GameWorld::Instance()->SetPaused(false);

        if (popup != nullptr)
        {
            popup->ShowMessage("Press I to open inventory", 3.0f);
        }
    }

    wasStartPressed = isStartPressed;
}

void GameUIComponent::HandlePauseInput()
{
    bool isPausePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

    if (isPausePressed && !wasPausePressed)
    {
        if (inventory != nullptr && inventory->IsOpen())
        {
            CloseInventory();
            Engine::GameWorld::Instance()->SetPaused(false);
            wasPausePressed = isPausePressed;
            return;
        }

        isPauseOpen = !isPauseOpen;

        if (isPauseOpen)
        {
            if (overlay != nullptr)
            {
                overlay->ShowPause();
            }

            Engine::GameWorld::Instance()->SetPaused(true);
        }
        else
        {
            if (overlay != nullptr)
            {
                overlay->HideOverlay();
            }

            Engine::GameWorld::Instance()->SetPaused(false);
        }
    }

    wasPausePressed = isPausePressed;
}

void GameUIComponent::HandleInventoryInput(sf::RenderWindow& window)
{
    bool isInventoryPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::I);

    if (isInventoryPressed && !wasInventoryPressed && inventory != nullptr)
    {
        ToggleInventory();
        Engine::GameWorld::Instance()->SetPaused(inventory->IsOpen());
    }

    wasInventoryPressed = isInventoryPressed;

    if (inventory == nullptr || !inventory->IsOpen())
    {
        return;
    }

    bool isLeftMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool isRightMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);

    sf::Vector2i pixelMousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosition =
        window.mapPixelToCoords(pixelMousePosition, window.getDefaultView());

    if (isRightMousePressed && !wasRightMousePressed && draggedItem.has_value())
    {
        ClearSelectedItem();
        wasRightMousePressed = isRightMousePressed;
        return;
    }

    wasRightMousePressed = isRightMousePressed;

    if (isLeftMousePressed && !wasLeftMousePressed)
    {
        if (inventory->TryChangePage(mousePosition))
        {
            wasLeftMousePressed = isLeftMousePressed;
            return;
        }

        std::optional<UIItemView> selectedItem = inventory->TryPickItem(mousePosition);

        if (selectedItem.has_value())
        {
            SelectItem(selectedItem.value());
            wasLeftMousePressed = isLeftMousePressed;
            return;
        }

        if (TryPlaceSelectedItem(mousePosition) || draggedItem.has_value())
        {
            ClearSelectedItem();
            wasLeftMousePressed = isLeftMousePressed;
            return;
        }
    }

    wasLeftMousePressed = isLeftMousePressed;
}

void GameUIComponent::HandleHotbarInput()
{
    if (hotbar == nullptr || popup == nullptr)
    {
        return;
    }

    HotbarUseResult useResult = hotbar->TryUseHotkey();

    if (useResult.state == HotbarUseState::Empty)
    {
        popup->ShowMessage("Empty slot", 1.5f);
        return;
    }

    if (useResult.state == HotbarUseState::Used)
    {
        if (playerInventory != nullptr)
        {
            playerInventory->RemoveOneItem(useResult.itemName);
        }

        popup->ShowMessage("Used: " + useResult.itemName, 1.5f);
    }
}

void GameUIComponent::HandleDeathState()
{
    if (isGameOver || playerObject == nullptr)
    {
        return;
    }

    Engine::StatsComponent* stats = playerObject->GetComponent<Engine::StatsComponent>();

    if (stats == nullptr || !stats->IsDead())
    {
        return;
    }

    isGameOver = true;
    wasRestartPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    if (inventory != nullptr && inventory->IsOpen())
    {
        CloseInventory();
    }

    ClearSelectedItem();

    if (overlay != nullptr)
    {
        overlay->ShowGameOver();
    }

    Engine::GameWorld::Instance()->SetPaused(true);
}

void GameUIComponent::HandleGameOverInput()
{
    bool isRestartPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    if (isRestartPressed && !wasRestartPressed)
    {
        LevelProgress::Reset();
        Engine::Engine::Instance()->RequestSceneRestart();
    }

    wasRestartPressed = isRestartPressed;
}

void GameUIComponent::HandleLevelCompleteInput()
{
    bool isNextLevelPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    if (isNextLevelPressed && !wasRestartPressed)
    {
        LevelProgress::Advance();
        Engine::Engine::Instance()->RequestSceneRestart();
    }

    wasRestartPressed = isNextLevelPressed;
}

void GameUIComponent::ToggleInventory()
{
    if (inventory == nullptr)
    {
        return;
    }

    inventory->Toggle();

    if (equipment != nullptr)
    {
        equipment->SetOpen(inventory->IsOpen());
    }

    if (!inventory->IsOpen())
    {
        ClearSelectedItem();
    }
}

void GameUIComponent::CloseInventory()
{
    if (inventory != nullptr && inventory->IsOpen())
    {
        inventory->Toggle();
    }

    if (equipment != nullptr)
    {
        equipment->SetOpen(false);
    }

    ClearSelectedItem();
}

void GameUIComponent::ClearSelectedItem()
{
    draggedItem.reset();

    if (inventory != nullptr)
    {
        inventory->ClearSelection();
    }

    if (descriptionPanel != nullptr)
    {
        descriptionPanel->Hide();
    }
}

void GameUIComponent::SelectItem(const UIItemView& item)
{
    draggedItem = item;

    if (descriptionPanel != nullptr)
    {
        descriptionPanel->ShowItem(item);
    }
}

bool GameUIComponent::TryPlaceSelectedItem(sf::Vector2f mousePosition)
{
    if (!draggedItem.has_value())
    {
        return false;
    }

    if (equipment != nullptr && equipment->ContainsPoint(mousePosition))
    {
        if (equipment->TryPlaceItem(mousePosition, draggedItem.value()))
        {
            ShowPopupMessage("Equipped: " + draggedItem->stack.GetName());
            return true;
        }

        ShowPopupMessage("Wrong equipment slot");
        return true;
    }

    if (hotbar != nullptr && hotbar->ContainsPoint(mousePosition))
    {
        if (hotbar->TryPlaceItem(mousePosition, draggedItem.value()))
        {
            ShowPopupMessage("Added to hotbar: " + draggedItem->stack.GetName());
            return true;
        }

        ShowPopupMessage("Only potions fit the hotbar");
        return true;
    }

    return false;
}

void GameUIComponent::ShowPopupMessage(const std::string& message, float duration)
{
    if (popup != nullptr)
    {
        popup->ShowMessage(message, duration);
    }
}

void GameUIComponent::DrawDraggedItem(sf::RenderWindow& window)
{
    if (!draggedItem.has_value())
    {
        return;
    }

    sf::Vector2i pixelMousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosition =
        window.mapPixelToCoords(pixelMousePosition, window.getDefaultView());

    sf::RectangleShape icon;
    icon.setPosition({mousePosition.x - 16.0f, mousePosition.y - 16.0f});
    icon.setSize({32.0f, 32.0f});

    sf::Color iconColor = draggedItem->iconColor;
    iconColor.a = 220;

    icon.setFillColor(iconColor);
    icon.setOutlineColor(sf::Color(255, 255, 255, 180));
    icon.setOutlineThickness(2.0f);

    window.draw(icon);
}
}  // namespace Roguelike
