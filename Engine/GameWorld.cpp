// @file GameWorld.cpp

#include "pch.h"
#include "GameWorld.h"

namespace Engine
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
    if (gameObject == nullptr)
    {
        return;
    }

    if (!IsGameObjectAlive(gameObject))
    {
        return;
    }

    auto iterator =
        std::find(markedToDestroyGameObjects.begin(), markedToDestroyGameObjects.end(), gameObject);

    if (iterator == markedToDestroyGameObjects.end())
    {
        markedToDestroyGameObjects.push_back(gameObject);
    }
}

bool GameWorld::IsGameObjectAlive(GameObject* gameObject) const
{
    if (gameObject == nullptr)
    {
        return false;
    }

    auto iterator = std::find(gameObjects.begin(), gameObjects.end(), gameObject);

    return iterator != gameObjects.end();
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
    gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),
                                     [gameObject](GameObject* obj) { return obj == gameObject; }),
                      gameObjects.end());

    delete gameObject;
}
}  // namespace Engine