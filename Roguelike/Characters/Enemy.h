// @file Enemy.h

#pragma once

#include "Character.h"

namespace Engine
{
class GameObject;
}

namespace Roguelike
{
namespace GameConfig
{
struct EnemyConfig;
}

class Enemy : public Character
{
   public:
    Enemy(Engine::GameObject* player, const GameConfig::EnemyConfig& config, float x, float y);
};
}  // namespace Roguelike
