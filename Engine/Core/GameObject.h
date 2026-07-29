// @file GameObject.h

#pragma once

#include "TransformComponent.h"

#include <SFML/Window/Event.hpp>

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace Engine
{
class TransformComponent;

class GameObject
{
   public:
    explicit GameObject(std::string newName);
    ~GameObject();

    const std::string& GetName() const;

    void HandleEvent(const sf::Event& event);
    void Update(float deltaTime);
    void Render();

    template <typename T>
    T* AddComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component.");

        if constexpr (std::is_same<T, TransformComponent>::value)
        {
            if (GetComponent<TransformComponent>() != nullptr)
            {
                return nullptr;
            }
        }

        auto component = std::make_unique<T>(this);
        T* newComponent = component.get();
        components.push_back(std::move(component));

        return newComponent;
    }

    template <typename T>
    T* GetComponent() const
    {
        for (const auto& component : components)
        {
            if (auto casted = dynamic_cast<T*>(component.get()))
            {
                return casted;
            }
        }
        return nullptr;
    }

    friend class GameWorld;

   private:
    std::string name;
    std::vector<std::unique_ptr<Component>> components;
};
}  // namespace Engine
