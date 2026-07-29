// @file ResourceSystem.cpp

#include "pch.h"
#include "ResourceSystem.h"

#include "Logger.h"

namespace Engine
{
ResourceSystem* ResourceSystem::Instance()
{
    static ResourceSystem resourceSystem;
    return &resourceSystem;
}

void ResourceSystem::LoadTexture(const std::string& name, const std::string& sourcePath,
                                 bool isSmooth)
{
    auto [texture, inserted] = textures.try_emplace(name);

    if (!inserted)
    {
        return;
    }

    if (texture->second.loadFromFile(sourcePath))
    {
        texture->second.setSmooth(isSmooth);
    }
    else
    {
        LOG_ERROR("Texture load failed: " + name + " from " + sourcePath);
        textures.erase(texture);
    }
}

bool ResourceSystem::HasTexture(const std::string& name) const
{
    return textures.find(name) != textures.end();
}

const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
{
    auto texture = textures.find(name);

    if (texture == textures.end())
    {
        LOG_ERROR("Texture not found: " + name);
        return nullptr;
    }

    return &texture->second;
}

void ResourceSystem::LoadSoundBuffer(const std::string& name, const std::string& sourcePath)
{
    auto [soundBuffer, inserted] = soundBuffers.try_emplace(name);

    if (!inserted)
    {
        return;
    }

    if (!soundBuffer->second.loadFromFile(sourcePath))
    {
        LOG_ERROR("Sound buffer load failed: " + name + " from " + sourcePath);
        soundBuffers.erase(soundBuffer);
    }
}

bool ResourceSystem::HasSoundBuffer(const std::string& name) const
{
    return soundBuffers.find(name) != soundBuffers.end();
}

const sf::SoundBuffer* ResourceSystem::GetSoundBufferShared(const std::string& name) const
{
    auto soundBuffer = soundBuffers.find(name);

    if (soundBuffer == soundBuffers.end())
    {
        LOG_ERROR("Sound buffer not found: " + name);
        return nullptr;
    }

    return &soundBuffer->second;
}

void ResourceSystem::Clear()
{
    textures.clear();
    soundBuffers.clear();
}
}  // namespace Engine
