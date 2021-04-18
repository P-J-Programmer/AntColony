#pragma once

#include <vector>

#include "Ant.h"
#include "GridPosition.h"

class FAntNest
{
public:
	FAntNest(FAntWorld& AntWorld, const FGridPosition& NestPosition);

public:
	void SpawnAnt();
	void SpawnAnts(unsigned int NumAnts);

	void Update(const float DeltaSeconds);
	void SubmitVerticesToBuffer(const FTextureManager& TextureManager, FRenderVertexData& RenderVertexData);

private:
	std::vector<FAnt> m_Ants;
	FAntWorld& m_AntWorld;
	FGridPosition m_Position;
};