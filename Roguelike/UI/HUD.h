// @file HUD.h

#pragma once

#include "UIElement.h"
#include "UIProgressBar.h"

#include <SFML/Graphics.hpp>

namespace Roguelike
{
class HUD : public Engine::UIElement
{
   public:
    explicit HUD(const sf::Font& font);

    void SetStats(float health, float maxHealth, float armor, float maxArmor);
    void SetObjective(int level, int aliveEnemies, int totalEnemies);
    void Draw(sf::RenderWindow& window) override;

   private:
    const sf::Font& font;

    sf::Text healthText;
    sf::Text armorText;
    sf::Text levelText;
    sf::Text objectiveText;

    Engine::UIProgressBar healthBar;
    Engine::UIProgressBar armorBar;

    float currentHealth = 100.0f;
    float currentArmor = 0.0f;
    int currentLevel = 1;
    int currentAliveEnemies = 0;
    int currentTotalEnemies = 0;
};
}  // namespace Roguelike
