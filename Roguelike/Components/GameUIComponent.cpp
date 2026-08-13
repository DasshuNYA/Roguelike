// @file GameUIComponent.cpp

#include "pch.h"
#include "GameUIComponent.h"

#include "Engine.h"
#include "EquipmentPanel.h"
#include "GameConfig.h"
#include "GameNotifications.h"
#include "GameObject.h"
#include "GameScreenOverlay.h"
#include "GameWorld.h"
#include "HotbarPanel.h"
#include "HUD.h"
#include "InventoryComponent.h"
#include "InventoryPanel.h"
#include "ItemDescriptionPanel.h"
#include "LevelProgress.h"
#include "PlayerMovementComponent.h"
#include "PlayerItemEffectsComponent.h"
#include "PlayerRunSnapshot.h"
#include "PopupMessage.h"
#include "RenderSystem.h"
#include "RangedAttackComponent.h"
#include "SaveSystem.h"
#include "StatsComponent.h"
#include "UITextureUtils.h"
#include "UIConfig.h"

#include <algorithm>

namespace Roguelike
{
namespace
{
float ClampPlayerArmor(float armor)
{
    return std::clamp(armor, 0.0f, GameConfig::PlayerEntity.character.stats.maxArmor);
}

std::string GetItemEffectMessage(ItemEffectResult result, const ItemData& itemData)
{
    switch (result)
    {
        case ItemEffectResult::HealthRestored:
            return "Health restored";
        case ItemEffectResult::AttackIncreased:
            return "Attack increased";
        case ItemEffectResult::SpeedIncreased:
            return "Speed increased";
        case ItemEffectResult::None:
            return "Used: " + std::string(itemData.name);
    }

    return "Used: " + std::string(itemData.name);
}
}  // namespace

GameUIComponent::GameUIComponent(Engine::GameObject* gameObject) : Component(gameObject)
{
    font.loadFromFile(GameConfig::MainUIFontPath);
    titleFont.loadFromFile(GameConfig::TitleFontPath);
    CreateUI();
    SetScreenState(ScreenState::MainMenu);
}

GameUIComponent::~GameUIComponent() = default;

void GameUIComponent::SetPlayer(Engine::GameObject* player)
{
    playerObject = player;
    playerInventory =
        playerObject != nullptr ? playerObject->GetComponent<InventoryComponent>() : nullptr;
    playerItemEffects = playerObject != nullptr
                            ? playerObject->GetComponent<PlayerItemEffectsComponent>()
                            : nullptr;
    playerMovement =
        playerObject != nullptr ? playerObject->GetComponent<PlayerMovementComponent>() : nullptr;
    playerStats =
        playerObject != nullptr ? playerObject->GetComponent<Engine::StatsComponent>() : nullptr;

    if (playerStats != nullptr)
    {
        // Stats drive HUD updates by event; the frame update only refreshes objective text.
        playerStats->AddStatsChangedListener([this](float health, float armor)
                                             { UpdateHUDStats(health, armor); });
        UpdateHUDStats(playerStats->GetHealth(), playerStats->GetArmor());
    }

    RestorePlayerRunState();
}

void GameUIComponent::SetLevelObjective(const std::vector<Engine::GameObject*>& enemies, int level)
{
    objectiveEnemies = enemies;
    levelNumber = level;
    totalEnemyCount = static_cast<int>(objectiveEnemies.size());
    aliveEnemyCount = totalEnemyCount;

    if (levelNumber > 1)
    {
        SetScreenState(ScreenState::Playing);
        ShowPopupMessage("Level " + std::to_string(levelNumber), UIConfig::Popup::LevelDuration);
    }
}

void GameUIComponent::HandleEvent(const sf::Event& event)
{
    if (screenState == ScreenState::Paused && overlay != nullptr)
    {
        sf::RenderWindow& window = Engine::RenderSystem::Instance()->GetMainWindow();
        PauseMenuAction action = overlay->HandlePauseEvent(event, window);

        if (action == PauseMenuAction::Resume)
        {
            if (playerObject != nullptr)
            {
                RangedAttackComponent* rangedAttack =
                    playerObject->GetComponent<RangedAttackComponent>();
                if (rangedAttack != nullptr)
                {
                    rangedAttack->SuppressUntilMouseReleased();
                }
            }

            SetScreenState(ScreenState::Playing);
            return;
        }

        if (action == PauseMenuAction::ExitGame)
        {
            window.close();
            return;
        }
    }

    if (event.type == sf::Event::KeyPressed)
    {
        HandleKeyPressed(event.key.code);
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::RenderWindow& window = Engine::RenderSystem::Instance()->GetMainWindow();
        HandleMouseButtonPressed(event.mouseButton, window);
    }
}

void GameUIComponent::Update(float deltaTime)
{
    UpdateLevelObjective(deltaTime);
    HandleDeathState();
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
    hud = &uiManager.CreateElement<HUD>(font, titleFont);
    hotbar = &uiManager.CreateElement<HotbarPanel>(font);
    inventory = &uiManager.CreateElement<InventoryPanel>(font);
    equipment = &uiManager.CreateElement<EquipmentPanel>(font);
    descriptionPanel = &uiManager.CreateElement<ItemDescriptionPanel>(font);
    popup = &uiManager.CreateElement<PopupMessage>(font);
    overlay = &uiManager.CreateElement<GameScreenOverlay>(font, titleFont);
}

void GameUIComponent::SetScreenState(ScreenState newState)
{
    screenState = newState;

    if (newState != ScreenState::Playing)
    {
        CloseInventory();
    }

    if (overlay != nullptr)
    {
        switch (newState)
        {
            case ScreenState::MainMenu:
                overlay->ShowMainMenu();
                break;
            case ScreenState::Paused:
                overlay->ShowPause();
                break;
            case ScreenState::GameOver:
                overlay->ShowGameOver();
                break;
            case ScreenState::LevelComplete:
                overlay->ShowLevelCleared(levelNumber + 1);
                break;
            case ScreenState::Playing:
                overlay->HideOverlay();
                break;
        }
    }

    SyncWorldPause();
}

bool GameUIComponent::IsGameplayScreen() const { return screenState == ScreenState::Playing; }

void GameUIComponent::SyncWorldPause()
{
    bool isInventoryOpen = inventory != nullptr && inventory->IsOpen();
    Engine::GameWorld::Instance()->SetPaused(!IsGameplayScreen() || isInventoryOpen);
}

void GameUIComponent::UpdateHUD()
{
    if (hud == nullptr)
    {
        return;
    }

    hud->SetObjective(levelNumber, aliveEnemyCount, totalEnemyCount);
}

void GameUIComponent::UpdateHUDStats(float health, float armor)
{
    if (hud == nullptr)
    {
        return;
    }

    hud->SetStats(health, GameConfig::PlayerEntity.character.stats.maxHealth,
                  ClampPlayerArmor(armor), GameConfig::PlayerEntity.character.stats.maxArmor);
}

void GameUIComponent::UpdateLevelObjective(float deltaTime)
{
    if (!IsGameplayScreen())
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

    SetScreenState(ScreenState::LevelComplete);
}

void GameUIComponent::UpdateInventory()
{
    if (inventory == nullptr || playerInventory == nullptr)
    {
        return;
    }

    const std::vector<ItemStack>& items = playerInventory->GetItems();
    inventory->SetItems(items);

    if (hotbar != nullptr)
    {
        hotbar->SetInventoryItems(items);
    }

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

        hotbar->TryAutoPlaceItem(item);
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

void GameUIComponent::HandleKeyPressed(sf::Keyboard::Key key)
{
    ScreenState previousState = screenState;
    HandleDeathState();
    if (previousState != ScreenState::GameOver && screenState == ScreenState::GameOver)
    {
        return;
    }

    switch (screenState)
    {
        case ScreenState::GameOver:
            if (key == sf::Keyboard::Space)
            {
                HandleGameOverRestartPressed();
            }
            return;

        case ScreenState::LevelComplete:
            if (key == sf::Keyboard::Space)
            {
                HandleLevelCompleteNextPressed();
            }
            return;

        case ScreenState::MainMenu:
            if (key == sf::Keyboard::Space)
            {
                HandleStartPressed();
            }
            return;

        case ScreenState::Paused:
            if (key == sf::Keyboard::Escape)
            {
                HandlePausePressed();
            }
            return;

        case ScreenState::Playing:
            break;
    }

    if (key == sf::Keyboard::Escape)
    {
        HandlePausePressed();
        return;
    }

    if (key == sf::Keyboard::I)
    {
        HandleInventoryTogglePressed();
        return;
    }

    HandleHotbarKey(key);
}

void GameUIComponent::HandleMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton,
                                               sf::RenderWindow& window)
{
    if (!IsGameplayScreen() || inventory == nullptr || !inventory->IsOpen())
    {
        return;
    }

    sf::Vector2f mousePosition =
        window.mapPixelToCoords({mouseButton.x, mouseButton.y}, window.getDefaultView());

    if (mouseButton.button == sf::Mouse::Right)
    {
        HandleInventoryCancelPressed();
        return;
    }

    if (mouseButton.button == sf::Mouse::Left)
    {
        HandleInventoryClick(mousePosition);
    }
}

void GameUIComponent::HandleStartPressed()
{
    SetScreenState(ScreenState::Playing);

    if (popup != nullptr)
    {
        popup->ShowMessage("Press I to open inventory", UIConfig::Popup::TutorialDuration);
    }
}

void GameUIComponent::HandlePausePressed()
{
    if (inventory != nullptr && inventory->IsOpen())
    {
        CloseInventory();
        SyncWorldPause();
        return;
    }

    SetScreenState(screenState == ScreenState::Paused ? ScreenState::Playing : ScreenState::Paused);
}

void GameUIComponent::HandleInventoryTogglePressed()
{
    if (inventory == nullptr)
    {
        return;
    }

    ToggleInventory();
    SyncWorldPause();
}

void GameUIComponent::HandleInventoryClick(sf::Vector2f mousePosition)
{
    if (inventory == nullptr)
    {
        return;
    }

    if (inventory->TryChangePage(mousePosition))
    {
        return;
    }

    std::optional<UIItemView> selectedItem = inventory->TryPickItem(mousePosition);

    if (selectedItem.has_value())
    {
        SelectItem(selectedItem.value());
        return;
    }

    if (TryPlaceSelectedItem(mousePosition) || draggedItem.has_value())
    {
        ClearSelectedItem();
    }
}

void GameUIComponent::HandleInventoryCancelPressed()
{
    if (draggedItem.has_value())
    {
        // Right click cancels drag-and-drop without touching inventory data.
        ClearSelectedItem();
    }
}

void GameUIComponent::HandleHotbarKey(sf::Keyboard::Key key)
{
    if (hotbar == nullptr || popup == nullptr)
    {
        return;
    }

    HotbarUseResult useResult = hotbar->TryUseHotkey(key);

    if (useResult.state == HotbarUseState::None)
    {
        return;
    }

    if (useResult.state == HotbarUseState::Empty || useResult.itemData == nullptr)
    {
        popup->ShowMessage("Empty slot", UIConfig::Popup::QuickDuration);
        return;
    }

    ItemEffectResult effectResult = playerItemEffects != nullptr
                                        ? playerItemEffects->ApplyConsumable(*useResult.itemData)
                                        : ItemEffectResult::None;

    if (playerInventory != nullptr)
    {
        playerInventory->RemoveOneItem(useResult.itemData);
    }

    popup->ShowMessage(GetItemEffectMessage(effectResult, *useResult.itemData),
                       UIConfig::Popup::QuickDuration);
}

void GameUIComponent::HandleDeathState()
{
    if (screenState == ScreenState::GameOver || playerObject == nullptr)
    {
        return;
    }

    if (playerStats == nullptr || !playerStats->IsDead())
    {
        return;
    }

    SetScreenState(ScreenState::GameOver);
}

void GameUIComponent::HandleGameOverRestartPressed()
{
    Engine::SaveSystem::Instance()->RemoveValue(PlayerRunSnapshotKey);
    LevelProgress::Reset();
    Engine::Engine::Instance()->RequestSceneRestart();
}

void GameUIComponent::HandleLevelCompleteNextPressed()
{
    SavePlayerRunState();
    LevelProgress::Advance();
    Engine::Engine::Instance()->RequestSceneRestart();
}

void GameUIComponent::RestorePlayerRunState()
{
    if (playerInventory == nullptr)
    {
        return;
    }

    std::optional<PlayerRunSnapshot> snapshot =
        Engine::SaveSystem::Instance()->GetValue<PlayerRunSnapshot>(PlayerRunSnapshotKey);

    if (!snapshot.has_value())
    {
        return;
    }

    playerInventory->SetItems(snapshot->inventoryItems);
    knownInventoryItems = snapshot->inventoryItems;

    if (hotbar != nullptr)
    {
        hotbar->SetSavedSlots(snapshot->hotbarSlots);
    }

    if (equipment != nullptr)
    {
        equipment->SetSavedSlots(snapshot->equipmentSlots);
    }

    if (playerStats != nullptr)
    {
        playerStats->SetStats(snapshot->health, ClampPlayerArmor(snapshot->armor));
        playerStats->SetAttackPower(snapshot->attackPower);
    }

    if (playerMovement != nullptr)
    {
        playerMovement->SetSpeed(snapshot->movementSpeed);
    }
}

void GameUIComponent::SavePlayerRunState()
{
    if (playerInventory == nullptr)
    {
        return;
    }

    PlayerRunSnapshot snapshot;
    snapshot.inventoryItems = playerInventory->GetItems();

    if (hotbar != nullptr)
    {
        snapshot.hotbarSlots = hotbar->GetSavedSlots();
    }

    if (equipment != nullptr)
    {
        snapshot.equipmentSlots = equipment->GetSavedSlots();
    }

    if (playerStats != nullptr)
    {
        snapshot.health = playerStats->GetHealth();
        snapshot.armor = ClampPlayerArmor(playerStats->GetArmor());
        snapshot.attackPower = playerStats->GetAttackPower();
    }

    if (playerMovement != nullptr)
    {
        snapshot.movementSpeed = playerMovement->GetSpeed();
    }

    Engine::SaveSystem::Instance()->SetValue(PlayerRunSnapshotKey, snapshot);
}

void GameUIComponent::ToggleInventory()
{
    if (inventory == nullptr)
    {
        return;
    }

    inventory->Toggle();

    // Equipment is a companion panel, so it follows inventory visibility.
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
        descriptionPanel->SetOpen(false);
    }

    if (equipment != nullptr)
    {
        equipment->ClearHighlightedItem();
    }

    if (hotbar != nullptr)
    {
        hotbar->ClearHighlightedItem();
    }
}

void GameUIComponent::SelectItem(const UIItemView& item)
{
    draggedItem = item;

    if (descriptionPanel != nullptr)
    {
        descriptionPanel->ShowItem(item);
    }

    if (equipment != nullptr)
    {
        equipment->SetHighlightedItem(draggedItem);
    }

    if (hotbar != nullptr)
    {
        hotbar->SetHighlightedItem(draggedItem);
    }
}

bool GameUIComponent::TryPlaceSelectedItem(sf::Vector2f mousePosition)
{
    if (!draggedItem.has_value())
    {
        return false;
    }

    return TryEquipSelectedItem(mousePosition) || TryAssignSelectedItemToHotbar(mousePosition);
}

bool GameUIComponent::TryEquipSelectedItem(sf::Vector2f mousePosition)
{
    if (!draggedItem.has_value() || equipment == nullptr ||
        !equipment->ContainsPoint(mousePosition))
    {
        return false;
    }

    EquipmentPlacementPreview preview =
        equipment->PreviewPlacement(mousePosition, draggedItem.value());

    if (!preview.handled)
    {
        return false;
    }

    if (!preview.canPlace || draggedItem->stack.data == nullptr)
    {
        ShowPopupMessage("Wrong equipment slot");
        return true;
    }

    ItemStack equippedItem{draggedItem->stack.data, 1};

    if (playerInventory == nullptr || !playerInventory->RemoveOneItem(equippedItem.data))
    {
        ShowPopupMessage("Item missing");
        return true;
    }

    if (preview.replacedItem.has_value() && !playerInventory->AddItem(preview.replacedItem.value()))
    {
        playerInventory->AddItem(equippedItem);
        ShowPopupMessage("Inventory is full");
        return true;
    }

    equipment->CommitPlacement(preview, draggedItem.value());
    if (playerItemEffects != nullptr)
    {
        playerItemEffects->ApplyEquipmentChange(equippedItem.data, preview.replacedItem.has_value()
                                                                       ? preview.replacedItem->data
                                                                       : nullptr);
    }
    ShowPopupMessage("Equipped: " + equippedItem.GetName());
    return true;
}

bool GameUIComponent::TryAssignSelectedItemToHotbar(sf::Vector2f mousePosition)
{
    if (!draggedItem.has_value() || hotbar == nullptr || !hotbar->ContainsPoint(mousePosition))
    {
        return false;
    }

    if (hotbar->TryPlaceItem(mousePosition, draggedItem.value()))
    {
        ShowPopupMessage("Added to hotbar: " + draggedItem->stack.GetName());
        return true;
    }

    ShowPopupMessage("Only potions fit the hotbar");
    return true;
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

    const std::string textureKey = GetItemTextureKey(draggedItem->stack);

    if (!textureKey.empty() &&
        UITextureUtils::DrawTexture(window, textureKey,
                                    {mousePosition.x - UIConfig::DraggedItem::TextureSize * 0.5f,
                                     mousePosition.y - UIConfig::DraggedItem::TextureSize * 0.5f,
                                     UIConfig::DraggedItem::TextureSize,
                                     UIConfig::DraggedItem::TextureSize},
                                    UIConfig::DraggedItem::TextureAlpha))
    {
        return;
    }

    sf::RectangleShape icon;
    icon.setPosition({mousePosition.x - UIConfig::DraggedItem::FallbackSize * 0.5f,
                      mousePosition.y - UIConfig::DraggedItem::FallbackSize * 0.5f});
    icon.setSize(
        {UIConfig::DraggedItem::FallbackSize, UIConfig::DraggedItem::FallbackSize});

    sf::Color iconColor = draggedItem->iconColor;
    iconColor.a = UIConfig::DraggedItem::FallbackAlpha;

    icon.setFillColor(iconColor);
    icon.setOutlineColor(UIConfig::DraggedItem::FallbackOutlineColor);
    icon.setOutlineThickness(UIConfig::DraggedItem::FallbackOutlineThickness);

    window.draw(icon);
}
}  // namespace Roguelike
