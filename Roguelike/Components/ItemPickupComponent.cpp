// @file ItemPickupComponent.cpp

#include "pch.h"
#include "ItemPickupComponent.h"

#include "GameConfig.h"
#include "GameNotifications.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "InventoryComponent.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "TransformComponent.h"
#include "UIItemView.h"

namespace Roguelike
{
namespace
{
bool RenderItemTexture(const ItemStack& item, sf::Vector2f center, float size)
{
    const std::string textureKey = GetItemTextureKey(item);

    if (textureKey.empty() || !Engine::ResourceSystem::Instance()->HasTexture(textureKey))
    {
        return false;
    }

    const sf::Texture* texture = Engine::ResourceSystem::Instance()->GetTextureShared(textureKey);
    sf::Vector2u textureSize = texture->getSize();

    if (textureSize.x == 0 || textureSize.y == 0)
    {
        return false;
    }

    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setOrigin({static_cast<float>(textureSize.x) * 0.5f,
                      static_cast<float>(textureSize.y) * 0.5f});
    sprite.setPosition(center);
    sprite.setScale(size / static_cast<float>(textureSize.x),
                    size / static_cast<float>(textureSize.y));

    Engine::RenderSystem::Instance()->Render(sprite);
    return true;
}
}  // namespace

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
    shadow.setSize({size * 1.55f, size * 0.42f});
    shadow.setOrigin({size * 0.775f, size * 0.21f});
    shadow.setPosition({position.x, position.y + size * 0.6f});
    shadow.setFillColor(sf::Color(0, 0, 0, 70));

    Engine::RenderSystem::Instance()->Render(shadow);

    if (!RenderItemTexture(item, {position.x, position.y}, size * 1.75f))
    {
        sf::RectangleShape icon;
        icon.setSize({size, size});
        icon.setOrigin({size / 2.0f, size / 2.0f});
        icon.setPosition({position.x, position.y});
        icon.setRotation(45.0f);
        icon.setFillColor(item.GetIconColor());
        icon.setOutlineColor(sf::Color(245, 225, 170, 220));
        icon.setOutlineThickness(2.0f);

        Engine::RenderSystem::Instance()->Render(icon);
    }
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
