#pragma once

#include <vector>

#include "GridGenerator.h"

namespace GridGeneratorUtility
{
	FTileFrequencies GenerateDefaultTileFrequencies();
	FGeneratorRules GenerateDefaultRules();
	FGridPositions PlaceNests(FGridGenerator& Generator, int NumNests);
	FGridPositions PlaceFoodSources(FGridGenerator& Generator, int NumFoodSources);
}