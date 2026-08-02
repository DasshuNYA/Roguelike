// @file Character.h

#pragma once

namespace Engine
{
class GameObject;
}

namespace Roguelike
{
namespace GameConfig
{
struct CharacterConfig;
}

class Character
{
   public:
    virtual ~Character() = default;

    Engine::GameObject* GetGameObject() const;

   protected:
    void BuildCharacter(const GameConfig::CharacterConfig& config, float x, float y);

    Engine::GameObject* gameObject = nullptr;
};
}  // namespace Roguelike
