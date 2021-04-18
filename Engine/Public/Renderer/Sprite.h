#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "Mesh.h"
#include "SubTexture.h"

class FOffsetBuffer;
class FTextureManager;

struct FSpriteTexture
{
	std::string m_SpriteSheet;
	std::string m_Sprite;
};

class FSprite
{
public:
	FSprite(FSpriteTexture SpriteTexture);
	void SetTransform(const glm::mat4& Transform);
	void AddVerticesToBuffer(const FTextureManager& TextureManager, std::vector<float>& OutBuffer) const;
	void AddIndicesToBuffer(FOffsetBuffer& OutBuffer) const;
	void SetSpriteTexture(const std::string& Texture);

private:
	FMesh m_Mesh;
	FSpriteTexture m_SpriteTexture;
};
