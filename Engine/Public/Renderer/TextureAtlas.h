#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

#include "Texture.h"
#include "Sprite.h"

class FTextureManager;

class FTextureAtlas
{
public:
	FTextureAtlas(const FTextureManager& TextureManager, const std::string& FilePath);

public:
	const FSubTexture* GetSubTexture(const std::string& SubTexture) const;
	bool IsValidSubTexture(const std::string& SubTexture) const;

private:
	void ParseHeader(const FTextureManager& TextureManager, std::stringstream& OutLayoutDataStream);
	void ParseAtlasData(const FTextureManager& TextureManager, std::stringstream& OutLayoutDataStream);

private:	
	std::unordered_map<std::string, FSubTexture> m_Atlas;

	std::string m_Texture;

	unsigned int m_Width = 0;
	unsigned int m_Height = 0;
};
