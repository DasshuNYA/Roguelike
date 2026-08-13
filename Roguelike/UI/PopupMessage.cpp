// @file PopupMessage.cpp

#include "pch.h"
#include "PopupMessage.h"

#include "UIConfig.h"
#include "UITextureUtils.h"
#include "UITextUtils.h"

#include <algorithm>

namespace Roguelike
{
PopupMessage::PopupMessage(const sf::Font& uiFont) : font(uiFont)
{
    // Fallback rectangle is used only when the popup texture is not loaded.
    background.setSize(UIConfig::Popup::FallbackSize);
    background.setPosition(UIConfig::Popup::Position);
    background.setFillColor(UIConfig::Popup::BackgroundColor);
    background.setOutlineColor(UIConfig::Popup::OutlineColor);
    background.setOutlineThickness(UIConfig::Popup::OutlineThickness);

    text.setFont(font);
    text.setCharacterSize(UIConfig::Popup::TextSize);
    text.setFillColor(sf::Color::White);
    text.setPosition(UIConfig::Popup::TextPosition);

    GetAnimation().SetAlpha(0.0f);
    Hide();
}

void PopupMessage::ShowMessage(const std::string& message, float duration)
{
    unsigned int characterSize = UIConfig::Popup::TextSize;
    text.setString(message);
    text.setCharacterSize(characterSize);

    while (characterSize > UIConfig::Popup::MinimumTextSize &&
           text.getLocalBounds().width > UIConfig::Popup::TextMaxWidth)
    {
        text.setCharacterSize(--characterSize);
    }

    if (text.getLocalBounds().width > UIConfig::Popup::TextMaxWidth)
    {
        text.setString(
            UITextUtils::FitTextToWidth(font, message, characterSize,
                                        UIConfig::Popup::TextMaxWidth));
    }

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
    float fadeProgress = static_cast<float>(alpha) / 255.0f;
    float lifeProgress = lifeTime > 0.0f ? std::clamp(timer / lifeTime, 0.0f, 1.0f) : 1.0f;
    float enterOffset = UIConfig::Popup::EnterOffsetY * (1.0f - fadeProgress);
    float exitOffset = UIConfig::Popup::ExitFloatY *
                       std::max(0.0f, lifeProgress - UIConfig::Popup::ExitStartProgress) /
                       (1.0f - UIConfig::Popup::ExitStartProgress);
    float yOffset = enterOffset + exitOffset;

    // Alpha is inherited from UIAnimation; color RGB stays constant while opacity changes.
    sf::Color backgroundColor = UIConfig::Popup::BackgroundColor;
    backgroundColor.a = static_cast<sf::Uint8>(
        (static_cast<float>(UIConfig::Popup::BackgroundColor.a) / 255.0f) * alpha);

    background.setPosition(
        {UIConfig::Popup::Position.x, UIConfig::Popup::Position.y + yOffset});
    background.setFillColor(backgroundColor);
    background.setOutlineColor(
        UIConfig::WithAlpha(UIConfig::Popup::OutlineColor, alpha));
    text.setPosition(
        {UIConfig::Popup::TextPosition.x, UIConfig::Popup::TextPosition.y + yOffset});
    text.setFillColor(UIConfig::WithAlpha(UIConfig::Popup::TextColor, alpha));

    if (!UITextureUtils::DrawTexture(window, "ui_popup_message",
                                     {background.getPosition().x, background.getPosition().y,
                                      background.getSize().x, background.getSize().y},
                                     alpha))
    {
        window.draw(background);
    }

    window.draw(text);
}
}  // namespace Roguelike
