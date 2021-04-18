#include "Renderer/ShaderManager.h"

#include <filesystem>

#include "Filesystem/FileLoading.h"
#include "Logger/Logger.h" 
#include "Utility/GlfwUtilities.h" 
#include "Renderer/Shader.h"

namespace
{
	const char* ShaderDirectory = "../Content/Shaders/";
}

FShaderManager::FShaderManager()
{
	LoadAllShaders(ShaderDirectory);
}


const FShader* FShaderManager::GetShader(const std::string& Name) const
{
	if (!IsValidShader(Name))
	{
		return nullptr;
	}
	return &m_Shaders.at(Name);
}

void FShaderManager::AddShader(const std::string& FilePath)
{
	std::string ShaderName = ExtractFileName(FilePath);
	if (!IsValidShader(ShaderName))
	{
		m_Shaders.emplace(ShaderName, FilePath);
	}
}

bool FShaderManager::IsValidShader(const std::string& Name) const
{
	return m_Shaders.find(Name) != m_Shaders.end();
}

void FShaderManager::LoadAllShaders(const std::string& DirectoryPath)
{
	using namespace std::filesystem;

	if (!IsValidDirectory(DirectoryPath))
	{
		return;
	}

	for (const directory_entry& Entry : directory_iterator(DirectoryPath))
	{
		AddShader(Entry.path().string());
	}
}
