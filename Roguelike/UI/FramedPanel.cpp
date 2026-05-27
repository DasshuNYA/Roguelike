// @file FramedPanel.cpp

#include "pch.h"
#include "FramedPanel.h"

namespace Roguelike
{
FramedPanel::FramedPanel(const sf::Font& uiFont) : font(uiFont)
{
    background.setFillColor(sf::Color(38, 54, 38, 236));
    background.setOutlineColor(sf::Color(145, 142, 86, 255));
    background.setOutlineThickness(3.0f);

    titleText.setFont(font);
    titleText.setCharacterSize(26);
    titleText.setFillColor(sf::Color(226, 210, 132));

    GetAnimation().SetAlpha(0.0f);
    Hide();
}

void FramedPanel::SetupFrame(sf::Vector2f position, sf::Vector2f size, const std::string& title)
{
    background.setPosition(position);
    background.setSize(size);

    titleText.setString(title);
    titleText.setPosition(position.x + 24.0f, position.y + 18.0f);
}

void FramedPanel::DrawFrame(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();

    background.setFillColor(sf::Color(38, 54, 38, alpha));
    background.setOutlineColor(sf::Color(145, 142, 86, alpha));
    titleText.setFillColor(sf::Color(226, 210, 132, alpha));

    window.draw(background);
    window.draw(titleText);
}

sf::Vector2f FramedPanel::GetPosition() const { return background.getPosition(); }

sf::Vector2f FramedPanel::GetSize() const { return background.getSize(); }
}  // namespace Roguelike
