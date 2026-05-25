// @file MazeNavigation.h

#pragma once

#include "Vector.h"

#include <vector>

namespace Roguelike
{
class MazeNavigation
{
   public:
    static MazeNavigation* Instance();

    void SetMap(const std::vector<std::vector<bool>>& walkableGrid, float tileSize);

    std::vector<Engine::Vector2Df> FindPath(const Engine::Vector2Df& startWorldPosition,
                                            const Engine::Vector2Df& targetWorldPosition);

   private:
    MazeNavigation() = default;

    bool IsInside(int x, int y) const;
    bool IsWalkable(int x, int y) const;

    Engine::Vector2Df CellToWorld(int x, int y) const;
    std::pair<int, int> WorldToCell(const Engine::Vector2Df& position) const;

   private:
    std::vector<std::vector<bool>> walkableGrid;

    int width = 0;
    int height = 0;

    float tileSize = 64.f;
};
}  // namespace Roguelike