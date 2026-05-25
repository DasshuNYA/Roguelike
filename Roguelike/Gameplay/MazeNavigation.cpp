// @file MazeNavigation.cpp

#include "pch.h"
#include "MazeNavigation.h"

#include <algorithm>
#include <cmath>
#include <queue>

namespace Roguelike
{
MazeNavigation* MazeNavigation::Instance()
{
    static MazeNavigation navigation;
    return &navigation;
}

void MazeNavigation::SetMap(const std::vector<std::vector<bool>>& newWalkableGrid,
                            float newTileSize)
{
    walkableGrid = newWalkableGrid;
    tileSize = newTileSize;

    height = static_cast<int>(walkableGrid.size());
    width = height > 0 ? static_cast<int>(walkableGrid[0].size()) : 0;
}

std::vector<Engine::Vector2Df> MazeNavigation::FindPath(
    const Engine::Vector2Df& startWorldPosition, const Engine::Vector2Df& targetWorldPosition)
{
    std::vector<Engine::Vector2Df> result;

    if (width == 0 || height == 0)
    {
        return result;
    }

    auto start = WorldToCell(startWorldPosition);
    auto target = WorldToCell(targetWorldPosition);

    if (!IsWalkable(start.first, start.second) || !IsWalkable(target.first, target.second))
    {
        return result;
    }

    if (start == target)
    {
        return result;
    }

    std::queue<std::pair<int, int>> queue;

    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

    std::vector<std::vector<std::pair<int, int>>> parent(
        height, std::vector<std::pair<int, int>>(width, {-1, -1}));

    queue.push(start);
    visited[start.second][start.first] = true;

    std::vector<std::pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    while (!queue.empty())
    {
        auto current = queue.front();
        queue.pop();

        if (current == target)
        {
            break;
        }

        for (const auto& direction : directions)
        {
            int nextX = current.first + direction.first;
            int nextY = current.second + direction.second;

            if (!IsWalkable(nextX, nextY))
            {
                continue;
            }

            if (visited[nextY][nextX])
            {
                continue;
            }

            visited[nextY][nextX] = true;
            parent[nextY][nextX] = current;
            queue.push({nextX, nextY});
        }
    }

    if (!visited[target.second][target.first])
    {
        return result;
    }

    std::vector<std::pair<int, int>> cells;
    std::pair<int, int> current = target;

    while (current != start)
    {
        cells.push_back(current);
        current = parent[current.second][current.first];
    }

    std::reverse(cells.begin(), cells.end());

    for (const auto& cell : cells)
    {
        result.push_back(CellToWorld(cell.first, cell.second));
    }

    return result;
}

bool MazeNavigation::IsInside(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool MazeNavigation::IsWalkable(int x, int y) const { return IsInside(x, y) && walkableGrid[y][x]; }

Engine::Vector2Df MazeNavigation::CellToWorld(int x, int y) const
{
    return {x * tileSize, y * tileSize};
}

std::pair<int, int> MazeNavigation::WorldToCell(const Engine::Vector2Df& position) const
{
    return {static_cast<int>(std::round(position.x / tileSize)),
            static_cast<int>(std::round(position.y / tileSize))};
}
}  // namespace Roguelike