// @file HUD.cpp

#include "pch.h"
#include "HUD.h"

#include "GameConfig.h"

namespace Roguelike
{
HUD::HUD(const sf::Font& uiFont) : font(uiFont)
{
    healthText.setFont(font);
    healthText.setCharacterSize(18);
    healthText.setPosition(24.0f, 18.0f);

    armorText.setFont(font);
    armorText.setCharacterSize(18);
    armorText.setPosition(24.0f, 52.0f);

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
    currentArmor = armor;

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
    healthText.setString("HP: " + std::to_string(static_cast<int>(currentHealth)));
    armorText.setString("Armor: " + std::to_string(static_cast<int>(currentArmor)));
    levelText.setString("Level: " + std::to_string(currentLevel));
    objectiveText.setString("Enemies: " + std::to_string(currentAliveEnemies) + "/" +
                            std::to_string(currentTotalEnemies));

    float rightMargin = static_cast<float>(GameConfig::WindowWidth) - 32.0f;
    sf::FloatRect levelBounds = levelText.getLocalBounds();
    sf::FloatRect objectiveBounds = objectiveText.getLocalBounds();
    levelText.setPosition(rightMargin - levelBounds.width, 18.0f);
    objectiveText.setPosition(rightMargin - objectiveBounds.width, 44.0f);

    healthText.setFillColor(ApplyAlpha(sf::Color::White));
    armorText.setFillColor(ApplyAlpha(sf::Color::White));
    levelText.setFillColor(ApplyAlpha(sf::Color::White));
    objectiveText.setFillColor(ApplyAlpha(sf::Color::White));

    healthBar.GetAnimation().SetAlpha(GetAlpha());
    armorBar.GetAnimation().SetAlpha(GetAlpha());

    healthBar.Draw(window);
    armorBar.Draw(window);

    window.draw(healthText);
    window.draw(armorText);
    window.draw(levelText);
    window.draw(objectiveText);
}
}  // namespace Roguelike
