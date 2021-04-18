#pragma once

#include <string>
#include <unordered_map>

#include "Glad/glad.h"

#include "Logger/Logger.h"

class FShader;

class FShaderManager
{
public:
	FShaderManager();
public:
	const FShader* GetShader(const std::string& Name) const;
	void AddShader(const std::string& FilePath);

	bool IsValidShader(const std::string& Name) const;

	template <typename T>
	bool SetUniform(const std::string& Shader, const std::string& Uniform, const T& Value)
	{
		if (!IsValidShader(Shader))
		{
			Log().Error("Shader is not valid:", Shader);
			return false;
		}

		return m_Shaders.at(Shader).SetUniform(Uniform, Value);
	}

private:
	void LoadAllShaders(const std::string& DirectoryPath);

private:
	std::unordered_map<std::string, FShader> m_Shaders;
};
