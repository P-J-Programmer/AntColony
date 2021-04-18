#pragma once

#include <string>

#include <glad/glad.h>
#include "glm/glm.hpp"

class FShader
{
public:
	FShader(const std::string& Filepath);
	~FShader();

	bool SetUniform(const std::string& Uniform, GLint Value);
	bool SetUniform(const std::string& Uniform, const glm::mat4& Value);
public:	
	void Bind() const;
	static void Unbind();

private:
	FShader(const FShader& Shader) = delete;
	FShader& operator=(const FShader& Shader) = delete;

	FShader(FShader&& Shader) = delete;
	FShader& operator=(FShader&& Shader) = delete;

private:
	void LoadShader(const std::string& Path);

	GLuint m_Id;
};
	