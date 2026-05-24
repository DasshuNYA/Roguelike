// @file GameHudComponent.h

#pragma once

#include "Character.h"
#include "Component.h"

#include <memory>
#include <vector>

namespace Engine
{
class GameObject;
class TextRendererComponent;
}  // namespace Engine

namespace Roguelike
{
// Displays player health, enemy count, win state and game over state.
class GameHudComponent : public Engine::Component
{
   public:
    GameHudComponent(Engine::GameObject* gameObject);

    void Update(float deltaTime) override;
    void Render() override;

    void SetPlayer(Engine::GameObject* newPlayer);

    void SetEnemies(const std::vector<std::unique_ptr<Character>>* newEnemies);

   private:
    int GetAliveEnemiesCount() const;
    bool IsPlayerDead() const;

   private:
    Engine::GameObject* player = nullptr;
    Engine::TextRendererComponent* textRenderer = nullptr;

    const std::vector<std::unique_ptr<Character>>* enemies = nullptr;
};
}  // namespace Roguelike