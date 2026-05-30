// @file UITextureUtils.cpp

#include "pch.h"
#include "UITextureUtils.h"

#include "ResourceSystem.h"

#include <algorithm>

namespace Roguelike::UITextureUtils
{
bool HasTexture(const std::string& textureKey)
{
    return Engine::ResourceSystem::Instance()->HasTexture(textureKey);
}

bool DrawTexture(sf::RenderWindow& window,
                 const std::string& textureKey,
                 sf::FloatRect bounds,
                 sf::Uint8 alpha,
                 sf::Color tint,
                 float fillRatio,
                 sf::BlendMode blendMode)
{
    Engine::ResourceSystem* resources = Engine::ResourceSystem::Instance();

    if (!resources->HasTexture(textureKey))
    {
        return false;
    }

    const sf::Texture* texture = resources->GetTextureShared(textureKey);
    sf::Vector2u textureSize = texture->getSize();

    if (textureSize.x == 0 || textureSize.y == 0)
    {
        return false;
    }

    fillRatio = std::clamp(fillRatio, 0.0f, 1.0f);
    const int sourceWidth =
        std::max(1, static_cast<int>(static_cast<float>(textureSize.x) * fillRatio));

    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setTextureRect({0, 0, sourceWidth, static_cast<int>(textureSize.y)});
    sprite.setPosition({bounds.left, bounds.top});
    sprite.setScale(bounds.width / static_cast<float>(textureSize.x),
                    bounds.height / static_cast<float>(textureSize.y));
    sprite.setColor(sf::Color(tint.r, tint.g, tint.b,
                              static_cast<sf::Uint8>((static_cast<float>(tint.a) / 255.0f) *
                                                     static_cast<float>(alpha))));

    window.draw(sprite, blendMode);
    return true;
}

bool DrawItemTexture(sf::RenderWindow& window,
                     const UIItemView& item,
                     sf::FloatRect bounds,
                     sf::Uint8 alpha)
{
    const std::string textureKey = GetItemTextureKey(item.stack);
    return !textureKey.empty() && DrawTexture(window, textureKey, bounds, alpha);
}
}  // namespace Roguelike::UITextureUtils
