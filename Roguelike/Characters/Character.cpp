// @file Character.cpp

#include "pch.h"
#include "Character.h"

#include "DeathComponent.h"
#include "DirectionalSpriteComponent.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "ResourceSystem.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "StatsComponent.h"
#include "TransformComponent.h"

namespace Roguelike
{
Engine::GameObject* Character::GetGameObject() const { return gameObject; }

void Character::BuildCharacter(const GameConfig::CharacterConfig& config, float x, float y)
{
    gameObject = Engine::GameWorld::Instance()->CreateGameObject(config.name);

    gameObject->GetComponent<Engine::TransformComponent>()->SetWorldPosition(x, y);

    auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
    const sf::Texture* texture =
        Engine::ResourceSystem::Instance()->GetTextureShared(config.textures.baseTextureKey);

    if (texture != nullptr)
    {
        renderer->SetTexture(*texture);
    }

    renderer->SetPixelSize(GameConfig::CharacterPixelSize, GameConfig::CharacterPixelSize);

    auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
    rigidbody->SetLinearDamping(config.movement.linearDamping);

    auto directionalSprite = gameObject->AddComponent<DirectionalSpriteComponent>();
    directionalSprite->SetTextures(config.textures.idleTextureKey, config.textures.downTextureKey,
                                   config.textures.rightTextureKey, config.textures.upTextureKey,
                                   config.textures.leftTextureKey);

    gameObject->AddComponent<Engine::SpriteColliderComponent>();

    auto stats = gameObject->AddComponent<Engine::StatsComponent>();
    stats->SetStats(config.stats.maxHealth, config.stats.maxArmor);
    stats->SetAttackPower(config.stats.attackPower);

    gameObject->AddComponent<Engine::DeathComponent>();
}
}  // namespace Roguelike
