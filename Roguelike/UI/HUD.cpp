// @file HUD.cpp

#include "pch.h"
#include "HUD.h"

#include "GameConfig.h"
#include "UIConfig.h"
#include "UITextUtils.h"
#include "UITextureUtils.h"

#include <algorithm>

namespace Roguelike
{
namespace
{
bool DrawTexturedBar(sf::RenderWindow& window,
                     sf::FloatRect bounds,
                     float value,
                     float maxValue,
                     sf::Uint8 alpha,
                     sf::Color fillTint)
{
    const float ratio = maxValue > 0.0f ? value / maxValue : 0.0f;
    const bool drewFrame =
        UITextureUtils::DrawTexture(window, "ui_hud_health_bar_frame", bounds, alpha);
    const bool drewFill =
        UITextureUtils::DrawTexture(window, "ui_hud_health_bar_fill", bounds, alpha, fillTint,
                                    ratio);
    return drewFrame && drewFill;
}

bool DrawArmorIcon(sf::RenderWindow& window, sf::FloatRect bounds, sf::Uint8 alpha)
{
    return UITextureUtils::DrawTexture(window, "ui_player_armor", bounds, alpha);
}

}  // namespace

HUD::HUD(const sf::Font& uiFont, const sf::Font& uiTitleFont)
    : font(uiFont), titleFont(uiTitleFont)
{
    healthText.setFont(font);
    healthText.setCharacterSize(UIConfig::HUD::HealthTextSize);
    healthText.setPosition(UIConfig::HUD::HealthTextPosition);

    armorText.setFont(font);
    armorText.setCharacterSize(UIConfig::HUD::ArmorTextSize);
    armorText.setPosition(UIConfig::HUD::ArmorTextPosition);

    // Quest title and objective text sizes.
    levelText.setFont(titleFont);
    levelText.setCharacterSize(UIConfig::HUD::QuestLevelTextSize);

    objectiveText.setFont(font);
    objectiveText.setCharacterSize(UIConfig::HUD::QuestObjectiveTextSize);

    healthBar.SetPosition(UIConfig::HUD::FallbackHealthBarPosition);
    healthBar.SetSize(UIConfig::HUD::FallbackHealthBarSize);
    healthBar.SetFillColor(UIConfig::HUD::FallbackHealthBarColor);

    armorBar.SetPosition(UIConfig::HUD::FallbackArmorBarPosition);
    armorBar.SetSize(UIConfig::HUD::FallbackArmorBarSize);
    armorBar.SetFillColor(UIConfig::HUD::FallbackArmorBarColor);
}

void HUD::SetStats(float health, float maxHealth, float armor, float maxArmor)
{
    currentHealth = health;
    maximumHealth = maxHealth;
    currentArmor = armor;
    maximumArmor = maxArmor;

    healthBar.SetValue(health, maxHealth);
    armorBar.SetValue(armor, maxArmor);
}

void HUD::SetObjective(int level, int aliveEnemies, int totalEnemies)
{
    // GameUIComponent passes alive/total enemies. HUD converts it to killed/total in Draw().
    currentLevel = level;
    currentAliveEnemies = aliveEnemies;
    currentTotalEnemies = totalEnemies;
}

void HUD::Draw(sf::RenderWindow& window)
{
    healthText.setString("");
    armorText.setString("");
    levelText.setString("LEVEL " + std::to_string(currentLevel));
    // Quest progress is displayed as killed enemies, e.g. "Kill enemies: 0/5".
    int killedEnemies = std::max(0, currentTotalEnemies - currentAliveEnemies);
    objectiveText.setString("Kill enemies: " + std::to_string(killedEnemies) + "/" +
                            std::to_string(currentTotalEnemies));

    const sf::FloatRect& questBounds = UIConfig::HUD::QuestPanelBounds;
    float questCenterX = questBounds.left + questBounds.width * 0.5f;
    UITextUtils::Center(
        levelText, {questCenterX, questBounds.top + UIConfig::HUD::QuestLevelOffsetY});
    UITextUtils::Center(
        objectiveText, {questCenterX, questBounds.top + UIConfig::HUD::QuestEnemiesOffsetY});

    healthText.setFillColor(ApplyAlpha(sf::Color::White));
    armorText.setFillColor(ApplyAlpha(sf::Color::White));
    levelText.setFillColor(ApplyAlpha(UIConfig::HUD::QuestLevelTextColor));
    objectiveText.setFillColor(ApplyAlpha(UIConfig::HUD::QuestObjectiveTextColor));

    sf::Uint8 alpha = GetAlphaByte();
    bool drewHealthBar =
        DrawTexturedBar(window, UIConfig::HUD::HealthBarBounds, currentHealth, maximumHealth, alpha,
                        sf::Color::White);
    bool drewArmor = false;
    int armorIconCount =
        static_cast<int>(std::min(std::max(0.0f, currentArmor),
                                  std::max(0.0f, maximumArmor)));

    if (UITextureUtils::HasTexture("ui_player_armor"))
    {
        for (int i = 0; i < armorIconCount; ++i)
        {
            DrawArmorIcon(window,
                          {UIConfig::HUD::ArmorStartPosition.x +
                               static_cast<float>(i) * UIConfig::HUD::ArmorIconGap,
                           UIConfig::HUD::ArmorStartPosition.y,
                           UIConfig::HUD::ArmorIconSize.x, UIConfig::HUD::ArmorIconSize.y},
                          alpha);
        }

        drewArmor = true;
    }

    if (!drewHealthBar || !drewArmor)
    {
        healthBar.GetAnimation().SetAlpha(GetAlpha());
        armorBar.GetAnimation().SetAlpha(GetAlpha());

        if (!drewHealthBar)
        {
            healthBar.Draw(window);
        }

        if (!drewArmor)
        {
            armorBar.Draw(window);
        }
    }

    UITextureUtils::DrawTexture(window, "ui_quest_list", UIConfig::HUD::QuestPanelBounds, alpha);
    window.draw(healthText);
    window.draw(armorText);
    window.draw(levelText);
    window.draw(objectiveText);
}
}  // namespace Roguelike
