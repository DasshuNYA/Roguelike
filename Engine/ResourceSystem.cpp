// @file ResourceSystem.cpp

#include "pch.h"
#include "ResourceSystem.h"

namespace XYZEngine
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
			delete newTexture;
		}
	}

	const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
	{
		std::map<std::string, sf::Texture*>::const_iterator texture = textures.find(name);

		if (texture == textures.end())
		{
			return nullptr;
		}

		return texture->second;
	}

	sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
	{
		std::map<std::string, sf::Texture*>::const_iterator texture = textures.find(name);

		if (texture == textures.end())
		{
			return nullptr;
		}

		return new sf::Texture(*texture->second);
	}

	void ResourceSystem::DeleteSharedTexture(const std::string& name)
	{
		std::map<std::string, sf::Texture*>::iterator texturePair = textures.find(name);

		if (texturePair == textures.end())
		{
			return;
		}

		sf::Texture* deletingTexture = texturePair->second;

		textures.erase(texturePair);

		delete deletingTexture;
	}

	void ResourceSystem::LoadTextureMap(
		const std::string& name,
		std::string sourcePath,
		sf::Vector2u elementPixelSize,
		int totalElements,
		bool isSmooth
	)
	{
		if (textureMaps.find(name) != textureMaps.end())
		{
			return;
		}

		sf::Texture textureMap;

		if (textureMap.loadFromFile(sourcePath))
		{
			std::vector<sf::Texture*> textureMapElements;

			sf::Vector2u textureSize = textureMap.getSize();

			const int textureWidth = static_cast<int>(textureSize.x);
			const int textureHeight = static_cast<int>(textureSize.y);

			const int elementWidth = static_cast<int>(elementPixelSize.x);
			const int elementHeight = static_cast<int>(elementPixelSize.y);

			int loadedElements = 0;

			for (int y = 0; y <= textureHeight - elementHeight; y += elementHeight)
			{
				if (loadedElements == totalElements)
				{
					break;
				}

				for (int x = 0; x <= textureWidth - elementWidth; x += elementWidth)
				{
					if (loadedElements == totalElements)
					{
						break;
					}

					sf::Texture* newTextureMapElement = new sf::Texture();

					if (newTextureMapElement->loadFromFile(
						sourcePath,
						sf::IntRect(x, y, elementWidth, elementHeight)
					))
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
	}

	const sf::Texture* ResourceSystem::GetTextureMapElementShared(
		const std::string& name,
		int elementIndex
	) const
	{
		std::map<std::string, std::vector<sf::Texture*>>::const_iterator textureMap =
			textureMaps.find(name);

		if (textureMap == textureMaps.end())
		{
			return nullptr;
		}

		if (elementIndex < 0 || elementIndex >= static_cast<int>(textureMap->second.size()))
		{
			return nullptr;
		}

		return textureMap->second[elementIndex];
	}

	sf::Texture* ResourceSystem::GetTextureMapElementCopy(
		const std::string& name,
		int elementIndex
	) const
	{
		std::map<std::string, std::vector<sf::Texture*>>::const_iterator textureMap =
			textureMaps.find(name);

		if (textureMap == textureMaps.end())
		{
			return nullptr;
		}

		if (elementIndex < 0 || elementIndex >= static_cast<int>(textureMap->second.size()))
		{
			return nullptr;
		}

		return new sf::Texture(*textureMap->second[elementIndex]);
	}

	int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
	{
		std::map<std::string, std::vector<sf::Texture*>>::const_iterator textureMap =
			textureMaps.find(name);

		if (textureMap == textureMaps.end())
		{
			return 0;
		}

		return static_cast<int>(textureMap->second.size());
	}

	void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
	{
		std::map<std::string, std::vector<sf::Texture*>>::iterator textureMap =
			textureMaps.find(name);

		if (textureMap == textureMaps.end())
		{
			return;
		}

		std::vector<sf::Texture*> deletingTextures = textureMap->second;

		for (size_t i = 0; i < deletingTextures.size(); i++)
		{
			delete deletingTextures[i];
		}

		textureMaps.erase(textureMap);
	}

	void ResourceSystem::Clear()
	{
		DeleteAllTextures();
		DeleteAllTextureMaps();
	}

	void ResourceSystem::DeleteAllTextures()
	{
		std::vector<std::string> keysToDelete;

		for (const std::pair<const std::string, sf::Texture*>& texturePair : textures)
		{
			keysToDelete.push_back(texturePair.first);
		}

		for (const std::string& key : keysToDelete)
		{
			DeleteSharedTexture(key);
		}
	}

	void ResourceSystem::DeleteAllTextureMaps()
	{
		std::vector<std::string> keysToDelete;

		for (const std::pair<const std::string, std::vector<sf::Texture*>>& textureMapPair : textureMaps)
		{
			keysToDelete.push_back(textureMapPair.first);
		}

		for (const std::string& key : keysToDelete)
		{
			DeleteSharedTextureMap(key);
		}
	}
}