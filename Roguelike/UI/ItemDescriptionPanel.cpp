// @file ItemDescriptionPanel.cpp

#include "pch.h"
#include "ItemDescriptionPanel.h"

#include "UITextUtils.h"

namespace Roguelike
{
ItemDescriptionPanel::ItemDescriptionPanel(const sf::Font& uiFont)
    : FramedPanel(uiFont), font(uiFont)
{
    SetupFrame({360.0f, 440.0f}, {420.0f, 120.0f}, "");

    icon.setPosition({384.0f, 478.0f});
    icon.setSize({44.0f, 44.0f});

    nameText.setFont(font);
    nameText.setCharacterSize(22);
    nameText.setPosition({448.0f, 466.0f});

    descriptionText.setFont(font);
    descriptionText.setCharacterSize(15);
    descriptionText.setPosition({448.0f, 520.0f});

    countText.setFont(font);
    countText.setCharacterSize(15);
    countText.setPosition({448.0f, 496.0f});

}

void ItemDescriptionPanel::ShowItem(const UIItemView& item)
{
    selectedItem = item;

    nameText.setString(UITextUtils::FitTextToWidth(font, selectedItem.stack.GetName(), 22, 300.0f));
    descriptionText.setString(
        UITextUtils::WrapText(font, selectedItem.stack.GetDescription(), 15, 300.0f, 2));
    countText.setString("Count: " + std::to_string(selectedItem.stack.count));

    icon.setFillColor(selectedItem.iconColor);

    Show();
}

void ItemDescriptionPanel::Draw(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();
    DrawFrame(window);

    sf::Color iconColor = selectedItem.iconColor;
    iconColor.a = alpha;
    icon.setFillColor(iconColor);

    nameText.setFillColor(sf::Color(255, 255, 255, alpha));
    descriptionText.setFillColor(sf::Color(220, 210, 190, alpha));
    countText.setFillColor(sf::Color(210, 200, 180, alpha));

    window.draw(icon);
    window.draw(nameText);
    window.draw(countText);
    window.draw(descriptionText);
}
}  // namespace Roguelike
