#pragma once

#include <unordered_map>
#include <string>

#include "Renderer/Texture.h"
#include "Renderer/TextureAtlas.h"

class FSubTexture;

class FTextureManager
{
public:
	FTextureManager();
public:
	const FTexture* GetTexture(const std::string& TextureName) const;
	void AddTexture(const std::string& FilePath);

	bool IsValidTexture(const std::string& TextureName) const;

	bool IsValidAtlas(const std::string& Atlas) const;
	bool IsValidSubTexture(const std::string& Atlas, const std::string& TextureName) const;
	const FSubTexture* GetSubTexture(const std::string& Atlas, const std::string& TextureName) const;

private:
	void LoadAllTextures(const std::string& DirectoryPath);
	void LoadAtlasData(const std::string& DirectoryPath);

private:
	std::unordered_map<std::string, FTexture> m_Textures;
	std::unordered_map<std::string, FTextureAtlas> m_Atlases;
};