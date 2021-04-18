#pragma once

class FFoodManager;
class FTileGrid;

class FAntWorld
{
public:
	FAntWorld(FTileGrid& TileGrid, FFoodManager& FoodManager) 
		: m_TileGrid(TileGrid)
		, m_FoodManager(FoodManager) {}
	FTileGrid& m_TileGrid;
	FFoodManager& m_FoodManager;
};