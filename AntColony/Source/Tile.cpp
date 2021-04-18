#include "Tile.h"

#include <string>
#include <unordered_map>

#include "Renderer/OffsetBuffer.h"

namespace
{
	static std::unordered_map<ETileType, std::string> s_Sprites =
	{
		{ETileType::Wall, "NestB"},
		{ETileType::Grass, "GrassA"},
		{ETileType::Mud, "Mud"},
		{ETileType::Nest, "NestA"},
		{ETileType::Food, "BushSmall"},
	};
}

void FTile::SetType(const ETileType TileType)
{
	m_TileType = TileType;
	m_Sprite.SetSpriteTexture(s_Sprites[TileType]);
}

bool FTile::IsNavigable() const
{
	static const std::vector<ETileType> NavigableTypes = { ETileType::Grass, ETileType::Food, ETileType::Nest };
	return std::find(NavigableTypes.begin(), NavigableTypes.end(), m_TileType) != NavigableTypes.end();
}

void FTile::SetTransform(const glm::mat4& Transform)
{
	m_Sprite.SetTransform(Transform);
}

void FTile::SubmitVerticesToBuffer(const FTextureManager& TextureManager, FRenderVertexData& RenderVertexData) const
{
	AddVerticesToBuffer(TextureManager, RenderVertexData.Vertices);
	AddIndicesToBuffer(RenderVertexData.Indices);
}

void FTile::AddVerticesToBuffer(const FTextureManager& TextureManager, std::vector<float>& OutBuffer) const
{
	m_Sprite.AddVerticesToBuffer(TextureManager, OutBuffer);
}

void FTile::AddIndicesToBuffer(FOffsetBuffer& OutBuffer) const
{
	m_Sprite.AddIndicesToBuffer(OutBuffer);
}

float FTile::GetPheromoneLevel(const EPheromoneType PheromoneType) const
{
	return m_PheromoneLevels[ToIndex(PheromoneType)];
}

void FTile::ModifyPheromoneLevel(const EPheromoneType PheromoneType, float Delta)
{
	m_PheromoneLevels[ToIndex(PheromoneType)] += Delta;
}

void FTile::ResetPheromoneLevels()
{
	for (std::size_t PheromoneIndex = 0; PheromoneIndex < NumPheromoneTypes; ++PheromoneIndex)
	{
		m_PheromoneLevels[PheromoneIndex] = 0;
	}
}
