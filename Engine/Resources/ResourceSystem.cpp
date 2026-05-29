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

void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth)
{
    if (textures.find(name) != textures.end())
    {
        return;
    }

    sf::Texture* newTexture = new sf::Texture();

    if (newTexture->loadFromFile(sourcePath))
    {
        newTexture->setSmooth(isSmooth);
        textures.emplace(name, newTexture);
    }
    else
    {
        LOG_ERROR("Texture load failed: " + name + " from " + sourcePath);
        delete newTexture;
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

    return texture->second;
}

sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
{
    const sf::Texture* texture = GetTextureShared(name);
    return texture != nullptr ? new sf::Texture(*texture) : nullptr;
}

void ResourceSystem::DeleteSharedTexture(const std::string& name)
{
    auto texturePair = textures.find(name);

    if (texturePair == textures.end())
    {
        return;
    }

    sf::Texture* deletingTexture = texturePair->second;
    textures.erase(texturePair);
    delete deletingTexture;
}

void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath,
                                    sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
{
    if (textureMaps.find(name) != textureMaps.end())
    {
        return;
    }

    sf::Texture textureMap;

    if (textureMap.loadFromFile(sourcePath))
    {
        std::vector<sf::Texture*> textureMapElements;

        auto textureSize = textureMap.getSize();

        if (textureSize.x < elementPixelSize.x || textureSize.y < elementPixelSize.y)
        {
            LOG_ERROR("Texture map is smaller than one element: " + name);
            return;
        }

        int loadedElements = 0;

        for (int y = 0; y <= static_cast<int>(textureSize.y - elementPixelSize.y);
             y += elementPixelSize.y)
        {
            if (loadedElements == totalElements)
            {
                break;
            }

            for (int x = 0; x <= static_cast<int>(textureSize.x - elementPixelSize.x);
                 x += elementPixelSize.x)
            {
                if (loadedElements == totalElements)
                {
                    break;
                }

                sf::Texture* newTextureMapElement = new sf::Texture();

                if (newTextureMapElement->loadFromFile(
                        sourcePath, sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
                {
                    newTextureMapElement->setSmooth(isSmooth);
                    textureMapElements.push_back(newTextureMapElement);
                }
                else
                {
                    delete newTextureMapElement;
                }

                loadedElements++;
            }
        }

        textureMaps.emplace(name, textureMapElements);
    }
    else
    {
        LOG_ERROR("Texture map load failed: " + name + " from " + sourcePath);
    }
}

const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name,
                                                              int elementIndex) const
{
    auto textureMap = textureMaps.find(name);

    if (textureMap == textureMaps.end())
    {
        LOG_ERROR("Texture map not found: " + name);
        return nullptr;
    }

    if (elementIndex < 0 || elementIndex >= static_cast<int>(textureMap->second.size()))
    {
        LOG_ERROR("Texture map element out of range: " + name);
        return nullptr;
    }

    return textureMap->second[elementIndex];
}

sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name,
                                                      int elementIndex) const
{
    const sf::Texture* texture = GetTextureMapElementShared(name, elementIndex);
    return texture != nullptr ? new sf::Texture(*texture) : nullptr;
}

int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
{
    auto textureMap = textureMaps.find(name);

    if (textureMap == textureMaps.end())
    {
        LOG_ERROR("Texture map not found: " + name);
        return 0;
    }

    return static_cast<int>(textureMap->second.size());
}

void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
{
    auto textureMap = textureMaps.find(name);

    if (textureMap == textureMaps.end())
    {
        return;
    }

    for (int i = 0; i < textureMap->second.size(); i++)
    {
        delete textureMap->second[i];
    }

    textureMaps.erase(textureMap);
}

void ResourceSystem::LoadSoundBuffer(const std::string& name, std::string sourcePath)
{
    if (soundBuffers.find(name) != soundBuffers.end())
    {
        return;
    }

    sf::SoundBuffer* newSoundBuffer = new sf::SoundBuffer();

    if (newSoundBuffer->loadFromFile(sourcePath))
    {
        soundBuffers.emplace(name, newSoundBuffer);
    }
    else
    {
        LOG_ERROR("Sound buffer load failed: " + name + " from " + sourcePath);
        delete newSoundBuffer;
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

    return soundBuffer->second;
}

void ResourceSystem::DeleteSharedSoundBuffer(const std::string& name)
{
    auto soundBufferPair = soundBuffers.find(name);

    if (soundBufferPair == soundBuffers.end())
    {
        return;
    }

    sf::SoundBuffer* deletingSoundBuffer = soundBufferPair->second;
    soundBuffers.erase(soundBufferPair);
    delete deletingSoundBuffer;
}

void ResourceSystem::Clear()
{
    DeleteAllTextures();
    DeleteAllTextureMaps();
    DeleteAllSoundBuffers();
}

void ResourceSystem::DeleteAllTextures()
{
    std::vector<std::string> keysToDelete;

    for (const auto& texturePair : textures)
    {
        keysToDelete.push_back(texturePair.first);
    }

    for (const auto& key : keysToDelete)
    {
        DeleteSharedTexture(key);
    }
}

void ResourceSystem::DeleteAllTextureMaps()
{
    std::vector<std::string> keysToDelete;

    for (const auto& textureMapPair : textureMaps)
    {
        keysToDelete.push_back(textureMapPair.first);
    }

    for (const auto& key : keysToDelete)
    {
        DeleteSharedTextureMap(key);
    }
}

void ResourceSystem::DeleteAllSoundBuffers()
{
    std::vector<std::string> keysToDelete;

    for (const auto& soundBufferPair : soundBuffers)
    {
        keysToDelete.push_back(soundBufferPair.first);
    }

    for (const auto& key : keysToDelete)
    {
        DeleteSharedSoundBuffer(key);
    }
}
}  // namespace Engine
