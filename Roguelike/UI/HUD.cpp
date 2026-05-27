// @file HUD.cpp

#include "pch.h"
#include "HUD.h"

#include "GameConfig.h"
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

// HUD layout. These are absolute screen coordinates.
const sf::FloatRect HealthBarBounds = {20.0f, 18.0f, 440.0f, 108.0f};
const sf::Vector2f ArmorStartPosition = {20.0f, 128.0f};
const sf::Vector2f ArmorIconSize = {64.0f, 64.0f};
const float ArmorIconGap = 72.0f;
const float ObjectiveRightMargin = 32.0f;
const float LevelTextY = 18.0f;
const float EnemiesTextY = 44.0f;
}  // namespace

HUD::HUD(const sf::Font& uiFont) : font(uiFont)
{
    healthText.setFont(font);
    healthText.setCharacterSize(24);
    healthText.setPosition(24.0f, 54.0f);

    armorText.setFont(font);
    armorText.setCharacterSize(24);
    armorText.setPosition(24.0f, 136.0f);

    levelText.setFont(font);
    levelText.setCharacterSize(18);

    objectiveText.setFont(font);
    objectiveText.setCharacterSize(18);

    healthBar.SetPosition({120.0f, 22.0f});
    healthBar.SetSize({180.0f, 18.0f});
    healthBar.SetFillColor(sf::Color(185, 55, 55, 255));

    armorBar.SetPosition({120.0f, 56.0f});
    armorBar.SetSize({180.0f, 18.0f});
    armorBar.SetFillColor(sf::Color(95, 125, 170, 255));
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
    currentLevel = level;
    currentAliveEnemies = aliveEnemies;
    currentTotalEnemies = totalEnemies;
}

void HUD::Draw(sf::RenderWindow& window)
{
    healthText.setString("");
    armorText.setString("");
    levelText.setString("Level: " + std::to_string(currentLevel));
    objectiveText.setString("Enemies: " + std::to_string(currentAliveEnemies) + "/" +
                            std::to_string(currentTotalEnemies));

    float rightMargin = static_cast<float>(GameConfig::WindowWidth) - ObjectiveRightMargin;
    sf::FloatRect levelBounds = levelText.getLocalBounds();
    sf::FloatRect objectiveBounds = objectiveText.getLocalBounds();
    levelText.setPosition(rightMargin - levelBounds.width, LevelTextY);
    objectiveText.setPosition(rightMargin - objectiveBounds.width, EnemiesTextY);

    healthText.setFillColor(ApplyAlpha(sf::Color::White));
    armorText.setFillColor(ApplyAlpha(sf::Color::White));
    levelText.setFillColor(ApplyAlpha(sf::Color::White));
    objectiveText.setFillColor(ApplyAlpha(sf::Color::White));

    sf::Uint8 alpha = GetAlphaByte();
    bool drewHealthBar =
        DrawTexturedBar(window, HealthBarBounds, currentHealth, maximumHealth, alpha,
                        sf::Color::White);
    bool drewArmor = false;
    int armorIconCount = static_cast<int>(std::max(0.0f, currentArmor));

    if (UITextureUtils::HasTexture("ui_player_armor"))
    {
        for (int i = 0; i < armorIconCount; ++i)
        {
            DrawArmorIcon(window,
                          {ArmorStartPosition.x + static_cast<float>(i) * ArmorIconGap,
                           ArmorStartPosition.y, ArmorIconSize.x, ArmorIconSize.y},
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

    window.draw(healthText);
    window.draw(armorText);
    window.draw(levelText);
    window.draw(objectiveText);
}
}  // namespace Roguelike
