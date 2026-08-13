// @file FramedPanel.cpp

#include "pch.h"
#include "FramedPanel.h"
#include "UIConfig.h"

namespace Roguelike
{
FramedPanel::FramedPanel(const sf::Font& uiFont, sf::FloatRect bounds, const std::string& title)
    : font(uiFont)
{
    background.setPosition({bounds.left, bounds.top});
    background.setSize({bounds.width, bounds.height});
    background.setFillColor(UIConfig::Frame::BackgroundColor);
    background.setOutlineColor(UIConfig::Frame::OutlineColor);
    background.setOutlineThickness(UIConfig::Frame::OutlineThickness);

    titleText.setFont(font);
    titleText.setString(title);
    titleText.setCharacterSize(UIConfig::Frame::TitleSize);
    titleText.setFillColor(UIConfig::Frame::TitleColor);
    titleText.setPosition(bounds.left + UIConfig::Frame::TitleOffset.x,
                          bounds.top + UIConfig::Frame::TitleOffset.y);

    GetAnimation().SetAlpha(0.0f);
    Hide();
}

void FramedPanel::SetOpen(bool open)
{
    isOpen = open;
    if (isOpen)
    {
        Show();
    }
    else
    {
        Hide();
    }
}

void FramedPanel::Toggle() { SetOpen(!isOpen); }

bool FramedPanel::IsOpen() const { return isOpen; }

bool FramedPanel::ContainsPoint(sf::Vector2f point) const
{
    return isOpen && GetBounds().contains(point);
}

void FramedPanel::DrawFrame(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();

    // The frame is redrawn every frame with current fade alpha from UIAnimation.
    background.setFillColor(UIConfig::WithAlpha(UIConfig::Frame::BackgroundColor, alpha));
    background.setOutlineColor(UIConfig::WithAlpha(UIConfig::Frame::OutlineColor, alpha));
    titleText.setFillColor(UIConfig::WithAlpha(UIConfig::Frame::TitleColor, alpha));

    window.draw(background);
    window.draw(titleText);
}

sf::FloatRect FramedPanel::GetBounds() const
{
    const sf::Vector2f position = background.getPosition();
    const sf::Vector2f size = background.getSize();
    return {position.x, position.y, size.x, size.y};
}

sf::Vector2f FramedPanel::GetPosition() const { return background.getPosition(); }

sf::Vector2f FramedPanel::GetSize() const { return background.getSize(); }
}  // namespace Roguelike
