#pragma once

#include <array>

#include "Renderer/Sprite.h"

#include "Pheromones.h"

class FTextureManager;
struct FRenderVertexData;

enum class ETileType : unsigned char
{
	Wall,
	Mud,
	Grass,
	Nest,
	Food,

	Num
};

class FTile
{
public:
	FTile(FSpriteTexture SpriteTexture) :
		m_PheromoneLevels(),
		m_Sprite(std::move(SpriteTexture)) {}

public:
	void SetType(const ETileType TileType);
	ETileType GetType() const { return m_TileType; }

	bool IsNavigable() const;

	void SetTransform(const glm::mat4& Transform);
	void SubmitVerticesToBuffer(const FTextureManager& TextureManager, FRenderVertexData& RenderVertexData) const;
	
	float GetPheromoneLevel(const EPheromoneType PheromoneType) const;
	void ModifyPheromoneLevel(const EPheromoneType PheromoneType, const float Delta);
	void ResetPheromoneLevels();

private:
	void AddVerticesToBuffer(const FTextureManager& TextureManager, std::vector<float>& OutBuffer) const;
	void AddIndicesToBuffer(FOffsetBuffer& OutBuffer) const;

private:
	std::array<float, NumPheromoneTypes> m_PheromoneLevels;
	FSprite m_Sprite;
	ETileType m_TileType = ETileType::Wall;
};
