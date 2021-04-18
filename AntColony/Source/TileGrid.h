#pragma once

#include "glm/glm.hpp"

#include "Ai/AStar.h"
#include "Core/Delegates.h"

#include "Tile.h"
#include "TileGridUtilities.h"

class FTextureManager;
class FOffsetBuffer;
struct FRenderVertexData;

enum class ENeighbourMode
{
	Moore, // Eight surrounding cell neighbours (immediate orthogonal/diagonal neighbours)
};

class FTileGrid : public IGraph<FGridPosition>
{
	friend class FGridGenerator;

public:
	FTileGrid(const FGridSettings& GridSettings);

	void SubmitVerticesToBuffer(const FTextureManager& TextureManager, FRenderVertexData& RenderVertexData) const;

	std::vector<const FTile*> GetNeighbours(const FGridPosition& GridPosition, const ENeighbourMode NeighbourMode) const;
	std::vector<const FTile*> GetNavigableNeighbours(const FGridPosition& GridPosition, const ENeighbourMode NeighbourMode) const;
	FGridPositions GetNeighbourPositions(const FGridPosition& GridPosition, const ENeighbourMode NeighbourMode) const;
	
	/* IGraph Interface */
	virtual FGridPositions GetNavigableNeighbourNodes(const FGridPosition& Node) const override;
	virtual float Cost(const FGridPosition& From, const FGridPosition& To) const override;
	virtual float Heuristic(const FGridPosition& From, const FGridPosition& To) const override;
	/* IGraph Interface */

	bool IsNavigable(const FGridPosition& GridPosition) const;

	bool IsValidPosition(const FGridPosition& GridPosition) const { return IsValidRow(GridPosition.Row) && IsValidCol(GridPosition.Col); }
	bool IsValidRow(int Row) const { return Row < m_Settings.Dimensions.NumRows; }
	bool IsValidCol(int Col) const { return Col < m_Settings.Dimensions.NumCols; }

	const FTile* GetTileAtPosition(const FGridPosition& GridPosition) const;
	FGridPosition GetPositionOfTile(const FTile* const Tile) const;

	void ModifyPheromoneLevel(const FGridPosition& GridPosition, const EPheromoneType Type, const float Delta);
	void Update(const float DeltaSeconds); // interface


	const FGridSettings& GetSettings() const { return m_Settings; }

	void Reset();

private:
	void CreateTiles();

	std::size_t GetTileIndex(const FGridPosition& GridPosition) const;
	std::size_t GetTileIndex(const FTile* const Tile) const;
	FTile* GetTileAtIndex(const std::size_t TileIndex);
	const FTile* GetTileAtIndex(const std::size_t TileIndex) const;
	
	FTile* GetTileAtPosition(const FGridPosition& GridPosition);

	FGridPosition GetPositionAtIndex(const std::size_t TileIndex) const;

private:
	std::vector<FTile> m_Tiles;
	FGridSettings m_Settings;
};
