// @file GameWorld.cpp

#include "pch.h"
#include "GameWorld.h"

#include <algorithm>
#include <utility>

namespace Engine
{
GameWorld* GameWorld::Instance()
{
    static GameWorld gameWorld;
    return &gameWorld;
}

void GameWorld::HandleEvent(const sf::Event& event)
{
    for (size_t i = 0; i < gameObjects.size(); i++)
    {
        if (ShouldUpdateGameObject(gameObjects[i].get()))
        {
            gameObjects[i]->HandleEvent(event);
        }
    }
}

void GameWorld::Update(float deltaTime)
{
    for (size_t i = 0; i < gameObjects.size(); i++)
    {
        if (ShouldUpdateGameObject(gameObjects[i].get()))
        {
            gameObjects[i]->Update(deltaTime);
        }
    }
}

void GameWorld::FixedUpdate(float deltaTime)
{
    if (isPaused)
    {
        return;
    }

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

GameObject* GameWorld::CreateGameObject(std::string name)
{
    auto gameObject = std::make_unique<GameObject>(std::move(name));
    GameObject* result = gameObject.get();
    gameObjects.push_back(std::move(gameObject));

    return result;
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

    return std::any_of(gameObjects.begin(), gameObjects.end(),
                       [gameObject](const auto& object) { return object.get() == gameObject; });
}

void GameWorld::SetPaused(bool value) { isPaused = value; }

bool GameWorld::IsPaused() const { return isPaused; }

void GameWorld::AddPauseIgnoredGameObject(GameObject* gameObject)
{
    if (gameObject == nullptr)
    {
        return;
    }

    auto iterator =
        std::find(pauseIgnoredGameObjects.begin(), pauseIgnoredGameObjects.end(), gameObject);

    if (iterator == pauseIgnoredGameObjects.end())
    {
        pauseIgnoredGameObjects.push_back(gameObject);
    }
}

void GameWorld::RemovePauseIgnoredGameObject(GameObject* gameObject)
{
    pauseIgnoredGameObjects.erase(
        std::remove(pauseIgnoredGameObjects.begin(), pauseIgnoredGameObjects.end(), gameObject),
        pauseIgnoredGameObjects.end());
}

void GameWorld::Clear()
{
    gameObjects.clear();
    markedToDestroyGameObjects.clear();
    pauseIgnoredGameObjects.clear();

    isPaused = false;
    fixedCounter = 0.0f;
}

bool GameWorld::ShouldUpdateGameObject(GameObject* gameObject) const
{
    if (!isPaused)
    {
        return true;
    }

    return IsPauseIgnored(gameObject);
}

bool GameWorld::IsPauseIgnored(GameObject* gameObject) const
{
    auto iterator =
        std::find(pauseIgnoredGameObjects.begin(), pauseIgnoredGameObjects.end(), gameObject);

    return iterator != pauseIgnoredGameObjects.end();
}

void GameWorld::DestroyGameObjectImmediate(GameObject* gameObject)
{
    RemovePauseIgnoredGameObject(gameObject);

    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
                       [gameObject](const auto& object) { return object.get() == gameObject; }),
        gameObjects.end());
}
}  // namespace Engine
