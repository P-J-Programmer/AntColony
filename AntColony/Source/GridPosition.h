#pragma once

#include <cmath>
#include <cstdlib>

struct FGridPosition
{
	int Row = 0;
	int Col = 0;
};

inline bool operator==(FGridPosition Left, FGridPosition Right)
{
	return (Left.Row == Right.Row) && (Left.Col == Right.Col);
}

inline bool operator!=(FGridPosition Left, FGridPosition Right)
{
	return !(Left == Right);
}

/* Implement std::hash function so we can put GridLocation into an unordered_set */
namespace std 
{
	template <> struct hash<FGridPosition> {
		std::size_t operator()(const FGridPosition& GridPosition) const
		{
			std::size_t HashRow = std::hash<int>{}(GridPosition.Row);
			std::size_t HashCol = std::hash<int>{}(GridPosition.Col);
			return HashRow ^ (HashCol << 1);
		}
	};
}

inline int ManhattanDistance(FGridPosition Left, FGridPosition Right)
{
	return std::abs(Left.Row - Right.Row) + std::abs(Left.Col - Right.Col);
}

inline int EuclidianDistanceSquare(FGridPosition Left, FGridPosition Right)
{
	return (Left.Row - Right.Row) * (Left.Row - Right.Row) + (Left.Col - Right.Col) * (Left.Col - Right.Col);
}

inline float EuclidianDistance(FGridPosition Left, FGridPosition Right)
{
	const float SquareDistance = static_cast<float>(EuclidianDistanceSquare(Left, Right));
	return std::sqrtf(SquareDistance);
}

using FGridPositions = std::vector<FGridPosition>;
