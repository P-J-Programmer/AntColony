#include "AntNest.h"

FAntNest::FAntNest(FAntWorld& AntWorld, const FGridPosition& NestPosition) 	
: m_AntWorld(AntWorld)
, m_Position(NestPosition)
{}

void FAntNest::SpawnAnt()
{
	m_Ants.emplace_back(m_AntWorld, FSpriteTexture{ "AntColonySpriteSheet", "Ant" }, m_Position);
}

void FAntNest::SpawnAnts(unsigned int NumAnts)
{
	for (unsigned int Ant = 0; Ant < NumAnts; ++Ant)
	{
		SpawnAnt();
	}
		
}

void FAntNest::SubmitVerticesToBuffer(const FTextureManager& TextureManager, FRenderVertexData& RenderVertexData)
{
	for (const FAnt& Ant : m_Ants)
	{
		Ant.SubmitVerticesToBuffer(TextureManager, RenderVertexData);
	}
}

void FAntNest::Update(const float DeltaSeconds)
{
	for (FAnt& Ant : m_Ants)
	{
		Ant.Update(DeltaSeconds);
	}
}
