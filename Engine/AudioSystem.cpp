// @file AudioSystem.cpp

#include <algorithm>
#include "pch.h"
#include "AudioSystem.h"

#include <iostream>

namespace XYZEngine
{
	AudioSystem* AudioSystem::Instance()
	{
		static AudioSystem instance;
		return &instance;
	}

	AudioSystem::AudioSystem()
	{
	}

	bool AudioSystem::LoadMusic(const std::string& name, const std::string& path)
	{
		auto it = musicMap.find(name);
		if (it != musicMap.end())
		{
			if (currentMusic == it->second.get())
				currentMusic = nullptr;
			musicMap.erase(it);
		}

		auto music = std::make_unique<sf::Music>();
		if (!music->openFromFile(path))
		{
			std::cerr << "[AudioSystem] Failed to load music: " << path << std::endl;
			return false;
		}

		music->setVolume(musicVolume);
		musicMap[name] = std::move(music);
		return true;
	}

	void AudioSystem::PlayMusic(const std::string& name, bool loop)
	{
		auto it = musicMap.find(name);
		if (it == musicMap.end())
		{
			std::cerr << "[AudioSystem] Music not loaded: " << name << std::endl;
			return;
		}

		if (currentMusic && currentMusic != it->second.get())
		{
			currentMusic->stop();
		}

		currentMusic = it->second.get();
		currentMusic->setLoop(loop);
		currentMusic->setVolume(musicVolume);
		currentMusic->play();
	}

	void AudioSystem::StopMusic()
	{
		if (currentMusic)
		{
			currentMusic->stop();
			currentMusic = nullptr;
		}
	}

	void AudioSystem::PauseMusic()
	{
		if (currentMusic && currentMusic->getStatus() == sf::Music::Playing)
		{
			currentMusic->pause();
		}
	}

	void AudioSystem::ResumeMusic()
	{
		if (currentMusic && currentMusic->getStatus() == sf::Music::Paused)
		{
			currentMusic->play();
		}
	}

	bool AudioSystem::IsMusicPlaying() const
	{
		return currentMusic && currentMusic->getStatus() == sf::Music::Playing;
	}

	void AudioSystem::SetMusicVolume(float volume)
	{
		musicVolume = std::clamp(volume, 0.0f, 100.0f);
		if (currentMusic)
		{
			currentMusic->setVolume(musicVolume);
		}
		// Optionally apply to all loaded tracks:
		/*
		for (auto& pair : musicMap)
		{
			pair.second->setVolume(musicVolume);
		}
		*/
	}

	float AudioSystem::GetMusicVolume() const
	{
		return musicVolume;
	}
}