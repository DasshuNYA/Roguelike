// @file PopupMessage.cpp

#include "pch.h"
#include "PopupMessage.h"

namespace Roguelike
{
PopupMessage::PopupMessage(const sf::Font& uiFont) : font(uiFont)
{
    background.setSize({320.0f, 44.0f});
    background.setPosition({20.0f, 120.0f});
    background.setFillColor(sf::Color(25, 22, 20, 210));
    background.setOutlineColor(sf::Color(120, 90, 65, 255));
    background.setOutlineThickness(2.0f);

    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    text.setPosition({38.0f, 130.0f});

    GetAnimation().SetAlpha(0.0f);
    Hide();
}

void PopupMessage::ShowMessage(const std::string& message, float duration)
{
    text.setString(message);

    lifeTime = duration;
    timer = 0.0f;

    Show();
}

void PopupMessage::Update(float deltaTime)
{
    Engine::UIElement::Update(deltaTime);

    if (!IsVisible())
    {
        return;
    }

    timer += deltaTime;

    if (timer >= lifeTime)
    {
        Hide();
    }
}

void PopupMessage::Draw(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();

    sf::Color backgroundColor = sf::Color(25, 22, 20, 210);
    backgroundColor.a = static_cast<sf::Uint8>((210.0f / 255.0f) * alpha);

    background.setFillColor(backgroundColor);
    background.setOutlineColor(sf::Color(120, 90, 65, alpha));
    text.setFillColor(sf::Color(255, 255, 255, alpha));

    window.draw(background);
    window.draw(text);
}
}  // namespace Roguelike
