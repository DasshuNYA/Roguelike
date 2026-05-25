// @file TextRendererComponent.h

#pragma once

#include "Component.h"

#include <SFML/Graphics.hpp>
#include <string>

namespace Engine
{
class TextRendererComponent : public Component
{
   public:
    TextRendererComponent(GameObject* gameObject);

    void Update(float deltaTime) override;
    void Render() override;

    void SetFont(const std::string& path);
    void SetText(const std::string& value);
    void SetCharacterSize(unsigned int size);
    void SetPosition(float x, float y);
    void SetColor(const sf::Color& color);

   private:
    sf::Font font;
    sf::Text text;

    bool isFontLoaded = false;
};
}  // namespace Engine