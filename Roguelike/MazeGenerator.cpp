// @file MazeGenerator.cpp

#include "pch.h"
#include "MazeGenerator.h"

#include "Floor.h"
#include "Wall.h"
#include "Logger.h"

#include <cstdlib>
#include <ctime>
#include <stack>

namespace Roguelike
{
	MazeGenerator::MazeGenerator(int newWidth, int newHeight)
		: width(newWidth), height(newHeight)
	{
		if (width % 2 == 0)
		{
			width++;
		}

		if (height % 2 == 0)
		{
			height++;
		}

		visited.resize(height, std::vector<bool>(width, false));
		isWall.resize(height, std::vector<bool>(width, true));
	}

	void MazeGenerator::Generate()
	{
		LOG_INFO("Maze generation started.");

		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		int startX = 1;
		int startY = 1;

		std::stack<std::pair<int, int>> stack;
		stack.push({ startX, startY });

		visited[startY][startX] = true;
		isWall[startY][startX] = false;

		while (!stack.empty())
		{
			auto [x, y] = stack.top();

			std::vector<std::pair<int, int>> directions =
				GetAvailableDirections(x, y);

			if (directions.empty())
			{
				stack.pop();
				continue;
			}

			auto direction =
				directions[std::rand() % directions.size()];

			int nextX = x + direction.first;
			int nextY = y + direction.second;

			CarvePath(x, y, nextX, nextY);

			visited[nextY][nextX] = true;

			stack.push({ nextX, nextY });
		}

		BuildObjects();

		LOG_INFO("Maze generation finished.");
	}

	float MazeGenerator::GetTileSize() const
	{
		return tileSize;
	}

	std::vector<std::pair<int, int>> MazeGenerator::GetAvailableDirections(int x, int y)
	{
		std::vector<std::pair<int, int>> directions =
		{
			{ 0, -2 },
			{ 0, 2 },
			{ -2, 0 },
			{ 2, 0 }
		};

		std::vector<std::pair<int, int>> available;

		for (auto direction : directions)
		{
			int nextX = x + direction.first;
			int nextY = y + direction.second;

			if (nextX <= 0 || nextY <= 0)
			{
				continue;
			}

			if (nextX >= width - 1 || nextY >= height - 1)
			{
				continue;
			}

			if (!visited[nextY][nextX])
			{
				available.push_back(direction);
			}
		}

		return available;
	}

	void MazeGenerator::CarvePath(int x1, int y1, int x2, int y2)
	{
		int wallX = (x1 + x2) / 2;
		int wallY = (y1 + y2) / 2;

		isWall[y1][x1] = false;
		isWall[wallY][wallX] = false;
		isWall[y2][x2] = false;
	}

	void MazeGenerator::BuildObjects()
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float worldX = x * tileSize;
				float worldY = y * tileSize;

				new Floor(worldX, worldY);

				if (isWall[y][x])
				{
					new Wall(worldX, worldY);
				}
			}
		}
	}
}