#include "Sprite.h"

#include "Logger/Logger.h"
#include "Renderer/OffsetBuffer.h"
#include "Renderer/SubTexture.h"
#include "Renderer/TextureManager.h"

FSprite::FSprite(FSpriteTexture SpriteTexture) :
	m_Mesh(QuadPositions()),
	m_SpriteTexture(std::move(SpriteTexture))
{}

void FSprite::SetTransform(const glm::mat4 & Transform)
{
	m_Mesh.SetTransform(Transform);
}

void FSprite::AddVerticesToBuffer(const FTextureManager& TextureManager, std::vector<float>& OutBuffer) const
{
	const FSubTexture* SubTexture = TextureManager.GetSubTexture(m_SpriteTexture.m_SpriteSheet, m_SpriteTexture.m_Sprite);
	if (SubTexture == nullptr)
	{
		Log().Error("FSprite: Missing subtexture");
		return;
	}

	const FMesh::FPositions& Positions = m_Mesh.GetWorldSpacePositions();
	const FSubTexture::FTextureCoordinates& TexCoords = SubTexture->GetTextureCoordinates();
	assert(Positions.size() == TexCoords.size());

	constexpr unsigned int FloatsPerVec = 6;
	OutBuffer.reserve(OutBuffer.size() + Positions.size() * FloatsPerVec); 

	for (std::size_t Index = 0; Index < Positions.size(); ++Index)
	{
		const glm::vec4& Position = Positions[Index];
		OutBuffer.push_back(Position.x);
		OutBuffer.push_back(Position.y);
		OutBuffer.push_back(Position.z);
		OutBuffer.push_back(Position.w);
		
		glm::vec2 Coordinate = TexCoords[Index];
		OutBuffer.push_back(Coordinate.x);
		OutBuffer.push_back(Coordinate.y);
	}
}

void FSprite::AddIndicesToBuffer(FOffsetBuffer& OutBuffer) const
{
	// Temp: These should be stored in the mesh
	OutBuffer.AddIndices({ 0, 1, 2, 2, 3, 0 });
}

void FSprite::SetSpriteTexture(const std::string& Texture)
{
	m_SpriteTexture.m_Sprite = Texture;
}

