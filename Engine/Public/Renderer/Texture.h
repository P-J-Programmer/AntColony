#pragma once

#include <string>

#include "glad/glad.h"

class FTexture
{
public:
	FTexture(const std::string& FilePath);
	~FTexture();

public:
	void Bind(GLuint Unit) const;

private:
	FTexture(const FTexture& Texture) = delete;
	FTexture& operator=(const FTexture& Texture) = delete;

	FTexture(FTexture&& Texture) = delete;
	FTexture& operator=(FTexture&& Texture) = delete;

private:
	GLuint m_Id;
};
