// @file AudioComponent.h

#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "Component.h"

namespace Engine
{
class AudioComponent : public Component
{
   public:
    AudioComponent(GameObject* gameObject);
    ~AudioComponent();

    void SetAudio(const sf::SoundBuffer& audio);
    void SetLoop(bool loop);

    void SetVolume(float volume);

    void Play();
    void Stop();

   private:
    sf::Sound* sound;
};
}  // namespace Engine
