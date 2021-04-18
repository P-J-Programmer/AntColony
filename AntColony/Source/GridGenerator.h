#pragma once

#include <array>
#include <map>

#include "TileGridUtilities.h"
#include "TileGrid.h"

class FTileGrid;

struct FTileFrequency
{
	ETileType Type;
	float Frequency;
};

struct FGeneratorRule
{
	using FNeighbourCountArray = std::array<unsigned int, static_cast<unsigned int>(ETileType::Num)>;
	FNeighbourCountArray NeighbourConditions {};
	ETileType Result = ETileType::Wall;
};

struct FGeneratorRules
{
public:
	void AddRule(const ETileType TileType, const FGeneratorRule& GeneratorRule);
	ETileType GetTypeForRule(const ETileType, FGeneratorRule::FNeighbourCountArray& Neighbours);
private:
	using FNeighbourRules = std::map<FGeneratorRule::FNeighbourCountArray, ETileType>;
	using FRulesPerTileType = std::array<FNeighbourRules, static_cast<unsigned int>(ETileType::Num)>;
	FRulesPerTileType m_Rules = {};

};

struct FSimpleRule
{
	/* Excluded tiles will not be changed by this rule*/
	std::vector<ETileType> ExcludedTiles;
	unsigned int MinNumNeighbours = 0;
	ETileType NeighbourType = ETileType::Wall;
};

class FTileFrequencies
{
public:
	FTileFrequencies(const std::vector<FTileFrequency>& RelativeFrequencies);
public:
	ETileType SelectRandom() const;

private:
	std::vector<FTileFrequency> m_CumulativeFrequencies;
};

class FGridGenerator
{
public:
	FGridGenerator(FTileGrid& TileGrid, const FGeneratorRules& Rules, const FTileFrequencies& InitialTileFrequencies);
public:
	void RunIteration();
	FGridPositions PlaceByRule(const ETileType TileType, const FSimpleRule& Rule, unsigned int NumToPlace);

private:
	void Initialise(const FTileFrequencies& InitialTileFrequencies);

private:
	FGeneratorRules m_Rules;
	FTileGrid& m_TileGrid;
	FGridDimensions m_Dimensions;
};