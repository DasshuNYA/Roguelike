// @file DeveloperLevel.h

#pragma once

#include "Character.h"
#include "Floor.h"
#include "Player.h"
#include "Scene.h"
#include "Wall.h"

#include <memory>
#include <vector>

namespace Roguelike
{
class DeveloperLevel : public Engine::Scene
{
   public:
    void Start() override;
    void Restart() override;
    void Stop() override;

   public:
    std::vector<std::unique_ptr<Floor>> floors;
    std::vector<std::unique_ptr<Wall>> walls;

   private:
    std::shared_ptr<Player> player;
    std::vector<std::unique_ptr<Character>> enemies;
};
}  // namespace Roguelike