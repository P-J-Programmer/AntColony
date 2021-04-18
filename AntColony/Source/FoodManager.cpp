#include "FoodManager.h"

#include <algorithm>

void FFoodManager::AddFoodSource(FFoodSource FoodSource)
{
	m_FoodSources.emplace_back(std::move(FoodSource));
}

void FFoodManager::AddFoodSources(const FGridPositions& Positions, const int AvailableFood)
{
	for (const FGridPosition& Position : Positions)
	{
		m_FoodSources.push_back({ Position, AvailableFood });
	}
}

void FFoodManager::ChangeFoodAmount(const FGridPosition& Position, const int FoodDelta)
{
	FFoodSource* FoodSource = GetSource(Position);
	if (FoodSource)
	{
		FoodSource->AvailableFood += FoodDelta;
	}
}

FGridPosition FFoodManager::ClosestFoodSource(const FGridPosition& Position)
{
	auto ClosestManhattanDistance = [Position](const FFoodSource& Left, const FFoodSource& Right)
	{
		return ManhattanDistance(Position, Left.Position) < ManhattanDistance(Position, Right.Position);
	};

	auto FoodIter = std::min_element(m_FoodSources.begin(), m_FoodSources.end(), ClosestManhattanDistance);
	if (FoodIter != m_FoodSources.end())
	{
		return FoodIter->Position;
	}

	return FGridPosition();
}

void FFoodManager::Reset()
{
	m_FoodSources.clear();
}

FFoodSource* FFoodManager::GetSource(const FGridPosition& Position)
{
	auto HasMatchingPosition = [Position](const FFoodSource& FoodSource)
	{
		return FoodSource.Position.Row == Position.Row && FoodSource.Position.Col == Position.Col;
	};

	auto FoodIter = std::find_if(m_FoodSources.begin(), m_FoodSources.end(), HasMatchingPosition);

	if (FoodIter != m_FoodSources.end())
	{
		return &(*FoodIter);
	}
	
	return nullptr;
}
