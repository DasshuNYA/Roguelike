// @file ItemDescriptionPanel.cpp

#include "pch.h"
#include "ItemDescriptionPanel.h"

#include "UIConfig.h"
#include "UITextureUtils.h"
#include "UITextUtils.h"

namespace Roguelike
{
ItemDescriptionPanel::ItemDescriptionPanel(const sf::Font& uiFont)
    : FramedPanel(uiFont, UIConfig::ItemDescription::PanelBounds), font(uiFont)
{
    nameText.setFont(font);
    nameText.setCharacterSize(UIConfig::ItemDescription::NameTextSize);
    nameText.setPosition(UIConfig::ItemDescription::NameTextPosition);

    descriptionText.setFont(font);
    descriptionText.setCharacterSize(UIConfig::ItemDescription::DescriptionTextSize);
    descriptionText.setPosition(UIConfig::ItemDescription::DescriptionTextPosition);

    countText.setFont(font);
    countText.setCharacterSize(UIConfig::ItemDescription::CountTextSize);
    countText.setPosition(UIConfig::ItemDescription::CountTextPosition);
}

void ItemDescriptionPanel::ShowItem(const UIItemView& item)
{
    selectedItem = item;

    // Long names/descriptions are clipped or wrapped here so they never leave the panel.
    nameText.setString(
        UITextUtils::FitTextToWidth(font, selectedItem.stack.GetName(),
                                    UIConfig::ItemDescription::NameTextSize,
                                    UIConfig::ItemDescription::NameMaxWidth));
    descriptionText.setString(
        UITextUtils::WrapText(font, selectedItem.stack.GetDescription(),
                              UIConfig::ItemDescription::DescriptionTextSize,
                              UIConfig::ItemDescription::DescriptionMaxWidth,
                              UIConfig::ItemDescription::DescriptionMaxLines));
    countText.setString("Count: " + std::to_string(selectedItem.stack.count));

    SetOpen(true);
}

void ItemDescriptionPanel::Draw(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();

    // Prefer the authored description texture; DrawFrame keeps the panel usable if art is missing.
    if (!UITextureUtils::DrawTexture(window, "ui_inventory_menu_description",
                                     UIConfig::ItemDescription::PanelBounds, alpha))
    {
        DrawFrame(window);
    }

    UITextureUtils::DrawItem(window, selectedItem, UIConfig::ItemDescription::IconBounds,
                             UIConfig::ItemDescription::IconBounds, alpha);

    nameText.setFillColor(
        UIConfig::WithAlpha(UIConfig::ItemDescription::NameTextColor, alpha));
    descriptionText.setFillColor(
        UIConfig::WithAlpha(UIConfig::ItemDescription::DescriptionTextColor, alpha));
    countText.setFillColor(
        UIConfig::WithAlpha(UIConfig::ItemDescription::CountTextColor, alpha));

    window.draw(nameText);
    window.draw(countText);
    window.draw(descriptionText);
}
}  // namespace Roguelike
