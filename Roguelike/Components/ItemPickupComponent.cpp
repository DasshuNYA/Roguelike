// @file ItemPickupComponent.cpp

#include "pch.h"
#include "ItemPickupComponent.h"

#include "GameConfig.h"
#include "GameNotifications.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "InventoryComponent.h"
#include "RenderSystem.h"
#include "TransformComponent.h"

namespace Roguelike
{
ItemPickupComponent::ItemPickupComponent(Engine::GameObject* gameObject)
    : PickupComponent(gameObject)
{
    transform = gameObject->GetComponent<Engine::TransformComponent>();
}

void ItemPickupComponent::SetItem(const ItemStack& newItem) { item = newItem; }

void ItemPickupComponent::Update(float deltaTime)
{
    if (transform == nullptr)
    {
        return;
    }

    Engine::Vector2Df position = transform->GetWorldPosition();
    float radius = GameConfig::ItemPickupRadius;

    bounds = sf::FloatRect(position.x - radius, position.y - radius, radius * 2.0f,
                           radius * 2.0f);
}

void ItemPickupComponent::Render()
{
    if (transform == nullptr || IsCollected())
    {
        return;
    }

    Engine::Vector2Df position = transform->GetWorldPosition();
    float size = GameConfig::ItemIconSize;

    sf::RectangleShape shadow;
    shadow.setSize({size, size * 0.35f});
    shadow.setOrigin({size / 2.0f, size * 0.175f});
    shadow.setPosition({position.x, position.y + size * 0.45f});
    shadow.setFillColor(sf::Color(0, 0, 0, 90));

    sf::RectangleShape icon;
    icon.setSize({size, size});
    icon.setOrigin({size / 2.0f, size / 2.0f});
    icon.setPosition({position.x, position.y});
    icon.setRotation(45.0f);
    icon.setFillColor(item.GetIconColor());
    icon.setOutlineColor(sf::Color(245, 225, 170, 220));
    icon.setOutlineThickness(2.0f);

    Engine::RenderSystem::Instance()->Render(shadow);
    Engine::RenderSystem::Instance()->Render(icon);
}

bool ItemPickupComponent::Collect(Engine::GameObject* collectorObject)
{
    InventoryComponent* inventory = collectorObject->GetComponent<InventoryComponent>();

    if (inventory == nullptr)
    {
        return false;
    }

    if (!inventory->AddItem(item))
    {
        GameNotifications::Push("Inventory is full");
        return false;
    }

    Engine::GameWorld::Instance()->DestroyGameObject(gameObject);
    return true;
}
}  // namespace Roguelike
