// @file GameWorld.cpp

#include "pch.h"
#include "GameWorld.h"

namespace XYZEngine
{
	GameWorld* GameWorld::Instance()
	{
		static GameWorld gameWorld;
		return &gameWorld;
	}

	void GameWorld::Update(float deltaTime)
	{
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Update(deltaTime);
		}
	}

	void GameWorld::FixedUpdate(float deltaTime)
	{
		fixedCounter += deltaTime;

		while (fixedCounter >= PhysicsSystem::Instance()->GetFixedDeltaTime())
		{
			PhysicsSystem::Instance()->Update();
			fixedCounter -= PhysicsSystem::Instance()->GetFixedDeltaTime();
		}
	}

	void GameWorld::Render()
	{
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Render();
		}
	}

	void GameWorld::LateUpdate()
	{
		for (size_t i = 0; i < markedToDestroyGameObjects.size(); i++)
		{
			DestroyGameObjectImmediate(markedToDestroyGameObjects[i]);
		}

		markedToDestroyGameObjects.clear();
	}

	GameObject* GameWorld::CreateGameObject()
	{
		GameObject* gameObject = new GameObject();
		gameObjects.push_back(gameObject);

		return gameObject;
	}

	GameObject* GameWorld::CreateGameObject(std::string name)
	{
		GameObject* gameObject = new GameObject(name);
		gameObjects.push_back(gameObject);

		return gameObject;
	}

	void GameWorld::DestroyGameObject(GameObject* gameObject)
	{
		markedToDestroyGameObjects.push_back(gameObject);
	}

	void GameWorld::Clear()
	{
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			delete gameObjects[i];
		}

		gameObjects.clear();
		markedToDestroyGameObjects.clear();
	}

	void GameWorld::Print() const
	{
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Print();
		}
	}

	void GameWorld::DestroyGameObjectImmediate(GameObject* gameObject)
	{
		gameObjects.erase(
			std::remove_if(
				gameObjects.begin(),
				gameObjects.end(),
				[gameObject](GameObject* obj)
				{
					return obj == gameObject;
				}
			),
			gameObjects.end()
		);

		delete gameObject;
	}
}