// @file MazeGenerator.h

#pragma once

#include <vector>

namespace Roguelike
{
class MazeGenerator
{
   public:
    MazeGenerator(int width, int height);

    void Generate();

    float GetTileSize() const;

   private:
    int width = 0;
    int height = 0;

    float tileSize = 64.f;

    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<bool>> isWall;

    std::vector<std::pair<int, int>> GetAvailableDirections(int x, int y);

    void CarvePath(int x1, int y1, int x2, int y2);
    void BuildObjects();
};
}  // namespace Roguelike