#include "Renderer/TextureManager.h"

#include <filesystem>

#include "Filesystem/FileLoading.h"
#include "Logger/Logger.h" 

#include "Renderer/SubTexture.h"

namespace
{
	const char* TextureDirectory = "../Content/Textures/"; 
	const char* AtlasDirectory = "../Content/TextureAtlasLayouts/";
}


FTextureManager::FTextureManager()
{
	LoadAllTextures(TextureDirectory);
	LoadAtlasData(AtlasDirectory);
}

const FTexture* FTextureManager::GetTexture(const std::string& TextureName) const
{
	if (!IsValidTexture(TextureName))
	{
		return nullptr;
	}
	return &m_Textures.at(TextureName);
}

void FTextureManager::AddTexture(const std::string& FilePath)
{
	std::string TextureName = ExtractFileName(FilePath);
	if (!IsValidTexture(TextureName))
	{
		m_Textures.emplace(TextureName, FilePath);
	}
}

bool FTextureManager::IsValidTexture(const std::string& TextureName) const
{
	return m_Textures.find(TextureName) != m_Textures.end();
}

bool FTextureManager::IsValidAtlas(const std::string& Atlas) const
{
	return m_Atlases.find(Atlas) != m_Atlases.end();
}

bool FTextureManager::IsValidSubTexture(const std::string& Atlas, const std::string& TextureName) const
{
	if (!IsValidAtlas(Atlas))
	{
		return false;
	}

	return m_Atlases.at(Atlas).IsValidSubTexture(TextureName);
}

const FSubTexture* FTextureManager::GetSubTexture(const std::string& Atlas, const std::string& TextureName) const
{
	if (!IsValidAtlas(Atlas))
	{
		return nullptr;
	}

	return m_Atlases.at(Atlas).GetSubTexture(TextureName);
}

void FTextureManager::LoadAllTextures(const std::string& DirectoryPath)
{
	using namespace std::filesystem;

	if (!IsValidDirectory(DirectoryPath))
	{
		return;
	}

	for (const directory_entry& Entry : directory_iterator(DirectoryPath))
	{
		AddTexture(Entry.path().string());
	}
}

void FTextureManager::LoadAtlasData(const std::string& DirectoryPath)
{
	using namespace std::filesystem;

	if (!IsValidDirectory(DirectoryPath))
	{
		return;
	}

	for (const directory_entry& Entry : directory_iterator(DirectoryPath))
	{
		FTextureAtlas TextureAtlas(*this, Entry.path().string());
		m_Atlases.emplace(ExtractFileName(Entry.path().string()), std::move(TextureAtlas));
	}
}
