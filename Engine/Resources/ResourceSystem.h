// @file ResourceSystem.h

#pragma once

#include <map>
#include <string>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace Engine
{
class ResourceSystem
{
   public:
    static ResourceSystem* Instance();

    void LoadTexture(const std::string& name, const std::string& sourcePath, bool isSmooth = true);
    bool HasTexture(const std::string& name) const;
    const sf::Texture* GetTextureShared(const std::string& name) const;

    void LoadSoundBuffer(const std::string& name, const std::string& sourcePath);
    bool HasSoundBuffer(const std::string& name) const;
    const sf::SoundBuffer* GetSoundBufferShared(const std::string& name) const;

    void Clear();

   private:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::SoundBuffer> soundBuffers;

    ResourceSystem() {}
    ~ResourceSystem() {}

    ResourceSystem(ResourceSystem const&) = delete;
    ResourceSystem& operator=(ResourceSystem const&) = delete;
};
}  // namespace Engine
