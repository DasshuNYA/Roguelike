// @file Player.h

#pragma once

#include "Character.h"

#include <vector>

namespace Engine
{
class GameObject;
}

namespace Roguelike
{
class Player : public Character
{
   public:
    Player();

    void AddAttackTarget(Engine::GameObject* target);
    void SetObstacles(const std::vector<Engine::GameObject*>& obstacles);
};
}  // namespace Roguelike
