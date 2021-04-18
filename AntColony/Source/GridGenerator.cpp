#include "GridGenerator.h"

#include <algorithm>
#include <numeric>

#include "Core/Random.h"

#include "TileGrid.h"

namespace
{
	FTileFrequency AccumulateFrequencies(const FTileFrequency& Lhs, const FTileFrequency& Rhs)
	{
		return { Rhs.Type, Lhs.Frequency + Rhs.Frequency };
	}

	constexpr std::size_t ToIndex(const ETileType TileType) { return static_cast<std::size_t>(TileType); }
}

FGridGenerator::FGridGenerator(FTileGrid& TileGrid, const FGeneratorRules& Rules, const FTileFrequencies& InitialTileFrequencies) :
	m_Rules(Rules),
	m_TileGrid(TileGrid),
	m_Dimensions(TileGrid.GetSettings().Dimensions)
{
	/*
	 * We leave a one cell border to avoid adding conditional 
	 * checks for edge tiles
	 */
	--m_Dimensions.NumCols;
	--m_Dimensions.NumRows;

	Initialise(InitialTileFrequencies);

	constexpr unsigned int NumIterations = 2;
	for (unsigned int Iteration = 0; Iteration < NumIterations; ++Iteration)
	{
		RunIteration();
	}
}

void FGridGenerator::RunIteration()
{
	static std::vector<ETileType> NewTypes;
	NewTypes.reserve((m_Dimensions.NumRows - 1) * (m_Dimensions.NumCols - 1));
	NewTypes.clear();

	for (int Row = 1; Row < m_Dimensions.NumRows; ++Row)
	{
		for (int Col = 1; Col < m_Dimensions.NumCols; ++Col)
		{
			auto Neighbours = m_TileGrid.GetNeighbours({Row, Col}, ENeighbourMode::Moore);
			FGeneratorRule::FNeighbourCountArray NeighbourCountArray = {};
			for (const FTile* NeighbourTile : Neighbours)
			{
				const ETileType NeighbourTileType = NeighbourTile->GetType();
				++NeighbourCountArray[static_cast<std::size_t>(NeighbourTileType)];
			}
			const ETileType OldTileType = m_TileGrid.GetTileAtPosition({ Row, Col })->GetType();
			const ETileType NewType = m_Rules.GetTypeForRule(OldTileType, NeighbourCountArray);
			if (NewType != ETileType::Num)
			{
				NewTypes.push_back(NewType);
			}
			else
			{
				NewTypes.push_back(OldTileType);
			}
		}
	}

	std::size_t Index = 0;
	for (int Row = 1; Row < m_Dimensions.NumRows; ++Row)
	{
		for (int Col = 1; Col < m_Dimensions.NumCols; ++Col)
		{
			const ETileType NewType = NewTypes[Index++];
			FTile* Tile = m_TileGrid.GetTileAtPosition({ Row, Col });
			if (Tile)
			{
				Tile->SetType(NewType);
			}			
		}
	}

}

FGridPositions FGridGenerator::PlaceByRule(const ETileType TileType, const FSimpleRule& Rule, unsigned int NumToPlace)
{
	FGridPositions GridPositions;
	GridPositions.reserve(NumToPlace);
	for (int Row = 1; Row < m_Dimensions.NumRows; ++Row)
	{
		for (int Col = 1; Col < m_Dimensions.NumCols; ++Col)
		{
			const FGridPosition Position = { Row, Col };
			
			if (GridPositions.size() >= NumToPlace)
			{
				return GridPositions;
			}

			FTile* const Tile = m_TileGrid.GetTileAtPosition(Position);
			if (std::find(Rule.ExcludedTiles.begin(), Rule.ExcludedTiles.end(), Tile->GetType()) != Rule.ExcludedTiles.end())
			{
				continue;
			}

			std::vector<const FTile*> Neighbours = m_TileGrid.GetNeighbours(Position, ENeighbourMode::Moore);
			FGeneratorRule::FNeighbourCountArray NeighbourCountArray = {};
			for (const FTile* NeighbourTile : Neighbours)
			{
				const ETileType NeighbourTileType = NeighbourTile->GetType();
				++NeighbourCountArray[ToIndex(NeighbourTileType)];
			} 

			if (NeighbourCountArray[ToIndex(Rule.NeighbourType)] >= Rule.MinNumNeighbours)
			{
				Tile->SetType(TileType);
				GridPositions.push_back(Position);
			}
		}
	}

	return GridPositions;
}

void FGridGenerator::Initialise(const FTileFrequencies& InitialTileFrequencies)
{
	for (int Row = 1; Row < m_Dimensions.NumRows; ++Row)
	{
		for (int Col = 1; Col < m_Dimensions.NumCols; ++Col)
		{
			FTile* Tile = m_TileGrid.GetTileAtPosition({ Row, Col });
			Tile->SetType(InitialTileFrequencies.SelectRandom());
		}
	}
}

FTileFrequencies::FTileFrequencies(const std::vector<FTileFrequency>& TileFrequencies) :
	m_CumulativeFrequencies(TileFrequencies.size())
{
	std::partial_sum(TileFrequencies.begin(), TileFrequencies.end(), m_CumulativeFrequencies.begin(), AccumulateFrequencies);
}

ETileType FTileFrequencies::SelectRandom() const
{
	if (m_CumulativeFrequencies.empty())
	{
		return ETileType::Wall;
	}

	const float SelectedFrequency = Random::RandInRange(0.0f, m_CumulativeFrequencies.back().Frequency);
	auto SelectedIter = std::upper_bound(
		m_CumulativeFrequencies.begin(),
		m_CumulativeFrequencies.end(), 
		SelectedFrequency,
		[](const float Lhs, const FTileFrequency& Rhs) { return Lhs < Rhs.Frequency; });
	return (SelectedIter != m_CumulativeFrequencies.end()) ? SelectedIter->Type : m_CumulativeFrequencies.back().Type;
}

void FGeneratorRules::AddRule(const ETileType TileType, const FGeneratorRule& GeneratorRule)
{
	assert(TileType != ETileType::Num);
	m_Rules[ToIndex(TileType)].emplace(GeneratorRule.NeighbourConditions, GeneratorRule.Result);
}

ETileType FGeneratorRules::GetTypeForRule(const ETileType TileType, FGeneratorRule::FNeighbourCountArray& Neighbours)
{
	auto RuleIter = m_Rules[ToIndex(TileType)].find(Neighbours);
	return (RuleIter != m_Rules[ToIndex(TileType)].end()) ? RuleIter->second : ETileType::Num;
}
