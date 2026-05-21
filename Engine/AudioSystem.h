//@file AudioSystem.h

#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <string>

namespace XYZEngine
{
	class AudioSystem
	{
	public:
		static AudioSystem* Instance();

		// Load a music file
		bool LoadMusic(const std::string& name, const std::string& path);

		// Playback control
		void PlayMusic(const std::string& name, bool loop = true);
		void StopMusic();
		void PauseMusic();
		void ResumeMusic();

		// State query
		bool IsMusicPlaying() const;

		// Volume (0..100)
		void SetMusicVolume(float volume);
		float GetMusicVolume() const;

	private:
		AudioSystem();
		~AudioSystem() = default;

		AudioSystem(const AudioSystem&) = delete;
		AudioSystem& operator=(const AudioSystem&) = delete;

		std::map<std::string, std::unique_ptr<sf::Music>> musicMap;
		sf::Music* currentMusic = nullptr; 
		float musicVolume = 100.0f;
	};
}