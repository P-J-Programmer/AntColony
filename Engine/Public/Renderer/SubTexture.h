#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "Renderer/Texture.h"

struct FBox
{
	const glm::vec2& Min;
	const glm::vec2& Max;
};

class FSubTexture
{
public:
	using FTextureCoordinates = std::vector<glm::vec2>;

public:
	FSubTexture(const FTexture& SpriteSheet, const FTextureCoordinates& TextureCoordinates);

public:
	const FTextureCoordinates& GetTextureCoordinates() const { return m_TextureCoordinates; };

private:
	const FTexture& m_SpriteSheet;
	FTextureCoordinates m_TextureCoordinates;
};