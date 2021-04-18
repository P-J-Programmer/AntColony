#include "TileGrid.h"

#include "GameConfig.h"

FTileGrid::FTileGrid(const FGridSettings& GridSettings) :
	m_Settings(GridSettings)
{
	CreateTiles();
}

void FTileGrid::SubmitVerticesToBuffer(const FTextureManager& TextureManager, FRenderVertexData& RenderVertexData) const
{
	for (const FTile& Tile : m_Tiles)
	{
		Tile.SubmitVerticesToBuffer(TextureManager, RenderVertexData);
	}
}

std::vector<const FTile*> FTileGrid::GetNeighbours(const FGridPosition& GridPosition, const ENeighbourMode NeighbourMode) const
{
	std::vector<const FTile*> Neighbours;
	const FGridPositions& NeighbourPositions = GetNeighbourPositions(GridPosition, NeighbourMode);
	Neighbours.reserve(NeighbourPositions.size());
	for (const FGridPosition& Position : NeighbourPositions)
	{
		const FTile* NeightbourTile = GetTileAtPosition(Position);
		if (NeightbourTile)
		{
			Neighbours.push_back(NeightbourTile);
		}
	}
	return Neighbours;
}

std::vector<const FTile*> FTileGrid::GetNavigableNeighbours(const FGridPosition& GridPosition, const ENeighbourMode NeighbourMode) const
{
	std::vector<const FTile*> Neighbours = GetNeighbours(GridPosition, NeighbourMode);
	Neighbours.erase(std::remove_if(Neighbours.begin(),
		Neighbours.end(),
		[](const FTile* Tile) { return !Tile->IsNavigable(); }),
		Neighbours.end());

	return Neighbours;
}


FGridPosition FTileGrid::GetPositionOfTile(const FTile* const Tile) const
{
	const std::size_t TileIndex = GetTileIndex(Tile);
	return GetPositionAtIndex(TileIndex);
}

void FTileGrid::ModifyPheromoneLevel(const FGridPosition& GridPosition, const EPheromoneType Type, const float Delta)
{
	FTile* Tile = GetTileAtPosition(GridPosition);
	if (Tile)
	{
		Tile->ModifyPheromoneLevel(Type, Delta);
	}
}

void FTileGrid::Update(const float DeltaSeconds)
{
	for (FTile& Tile : m_Tiles)
	{
		Tile.ModifyPheromoneLevel(EPheromoneType::Attractive, EvaporationRatePerSecond * DeltaSeconds);
	}
}

bool FTileGrid::IsNavigable(const FGridPosition& GridPosition) const
{
	const FTile* Tile = GetTileAtPosition(GridPosition);
	return Tile && Tile->IsNavigable();
}

const FTile* FTileGrid::GetTileAtPosition(const FGridPosition& GridPosition) const
{
	if (!IsValidPosition(GridPosition))
	{
		return nullptr;
	}
	const std::size_t TileIndex = GetTileIndex(GridPosition);
	return GetTileAtIndex(TileIndex);
}

void FTileGrid::Reset()
{
	for (FTile& Tile : m_Tiles)
	{
		Tile.SetType(ETileType::Wall);
		Tile.ResetPheromoneLevels();
	}
}

void FTileGrid::CreateTiles()
{
	const FGridDimensions& Dimensions = m_Settings.Dimensions;

	const unsigned int NumTiles = Dimensions.NumRows * Dimensions.NumCols;
	m_Tiles.reserve(NumTiles);

	for (int Row = 0; Row < Dimensions.NumRows; ++Row)
	{
		for (int Col = 0; Col < Dimensions.NumCols; ++Col)
		{
			m_Tiles.emplace_back(FSpriteTexture{ "AntColonySpriteSheet", "NestB" });
			m_Tiles.back().SetTransform(GetTileCentreTransform({ Row, Col }, m_Settings));
		}
	}
}

std::size_t FTileGrid::GetTileIndex(const FGridPosition& GridPosition) const
{
	const FGridDimensions& Dimensions = m_Settings.Dimensions;
	const int TileIndex = GridPosition.Row * Dimensions.NumCols + GridPosition.Col;
	return static_cast<std::size_t>(TileIndex);
}

std::size_t FTileGrid::GetTileIndex(const FTile* const Tile) const
{
	auto TileIter = std::find_if(m_Tiles.begin(), m_Tiles.end(), [Tile](const FTile& ThisTile) { return Tile == &ThisTile; });
	assert(TileIter != m_Tiles.end());
	return std::distance(m_Tiles.begin(), TileIter);
}

FTile* FTileGrid::GetTileAtIndex(const std::size_t TileIndex)
{
	return const_cast<FTile*>(const_cast<const FTileGrid*>(this)->GetTileAtIndex(TileIndex));
}

const FTile* FTileGrid::GetTileAtIndex(const std::size_t TileIndex) const
{
	if (TileIndex >= m_Tiles.size())
	{
		return nullptr;
	}
	return &m_Tiles[TileIndex];
}

FTile* FTileGrid::GetTileAtPosition(const FGridPosition& GridPosition)
{
	return const_cast<FTile*>(const_cast<const FTileGrid*>(this)->GetTileAtPosition(GridPosition));
}

FGridPosition FTileGrid::GetPositionAtIndex(const std::size_t TileIndex) const
{
	const FGridDimensions& Dimensions = m_Settings.Dimensions;
	assert(Dimensions.NumCols != 0);
	return { (int)TileIndex / Dimensions.NumCols, (int)TileIndex % Dimensions.NumCols };
}

FGridPositions FTileGrid::GetNeighbourPositions(const FGridPosition& GridPosition, const ENeighbourMode /*NeighbourMode*/) const
{
	constexpr std::size_t NumNeighbours = 8;
	FGridPositions NeighbourPositions;
	NeighbourPositions.reserve(NumNeighbours);

	for (int RowOffset = -1; RowOffset <= 1; ++RowOffset)
	{
		for (int ColOffset = -1; ColOffset <= 1; ++ColOffset)
		{
			if (RowOffset == 0 && ColOffset == 0)
			{
				continue;
			}

			FGridPosition NeighbourPosition(GridPosition);
			NeighbourPosition.Row += RowOffset;
			NeighbourPosition.Col += ColOffset;
			if (IsValidPosition(GridPosition))
			{
				NeighbourPositions.push_back(NeighbourPosition);
			}
		}
	}
	
	return NeighbourPositions;
}

/* IGraph Interface */
FGridPositions FTileGrid::GetNavigableNeighbourNodes(const FGridPosition& Node) const
{
	FGridPositions NeighbourPositions = GetNeighbourPositions(Node, ENeighbourMode::Moore);
	NeighbourPositions.erase(std::remove_if(NeighbourPositions.begin(),
		NeighbourPositions.end(),
		[this](const FGridPosition Position)
		{
			const FTile* Tile = GetTileAtPosition(Position);
			return Tile == nullptr || !Tile->IsNavigable();
		}),
		NeighbourPositions.end());

	return NeighbourPositions;

}

float FTileGrid::Cost(const FGridPosition& From, const FGridPosition& To) const
{
	return EuclidianDistance(From, To);
}

float FTileGrid::Heuristic(const FGridPosition& From, const FGridPosition& To) const
{
	return static_cast<float>(EuclidianDistanceSquare(From, To));
}
/* IGraph Interface */
