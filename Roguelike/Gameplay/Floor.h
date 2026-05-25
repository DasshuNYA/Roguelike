#pragma once

namespace Engine
{
class GameObject;
}

namespace Roguelike
{
class Floor
{
   public:
    Floor(float x, float y);

   private:
    Engine::GameObject* gameObject = nullptr;
};
}  // namespace Roguelike