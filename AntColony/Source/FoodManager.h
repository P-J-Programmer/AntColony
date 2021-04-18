#pragma once

#include <vector>

#include "GridPosition.h"

class FTileGrid;

struct FFoodSource
{
	FGridPosition Position;
	int AvailableFood = 0;
};

class FFoodManager
{
public:
	FFoodManager(FTileGrid& TileGrid) : m_TileGrid(TileGrid) {}

public:
	void AddFoodSource(FFoodSource FoodSource);
	void AddFoodSources(const FGridPositions& Positions, const int AvailableFood);
	void ChangeFoodAmount(const FGridPosition& Position, const int FoodDelta);
	FGridPosition ClosestFoodSource(const FGridPosition& Position);


	void Reset();
private:
	FFoodSource* GetSource(const FGridPosition& Position);

private:
	std::vector<FFoodSource> m_FoodSources;

	FTileGrid& m_TileGrid;
};