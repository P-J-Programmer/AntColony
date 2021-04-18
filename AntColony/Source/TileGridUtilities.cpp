#include "TileGridUtilities.h"

#include "glm/gtc/matrix_transform.hpp"

#include "GameConfig.h"

/*
 * Local space (0,0) is origin
 */
glm::vec2 PositionLocalSpace(const FGridPosition& GridPosition, const FGridSettings& GridSettings)
{
	const float XOffset = GridSettings.TileScale * static_cast<float>(GridPosition.Col);
	const float YOffset = GridSettings.TileScale * static_cast<float>(GridPosition.Row);
	return { XOffset, YOffset };
}


glm::vec2 GridCentreLocalSpace(const FGridSettings& GridSettings)
{
	/*
	 * Note: Centre of the grid does not necessarily correspond to a tile position if grid has an odd dimension
	 */
	const float XOffset = GridSettings.TileScale * static_cast<float>(GridSettings.Dimensions.NumCols) / 2.0f;
	const float YOffset = GridSettings.TileScale * static_cast<float>(GridSettings.Dimensions.NumRows) / 2.0f;
	return { XOffset, YOffset };
}

glm::vec2 OffsetFromOriginLocalSpace(const FGridPosition& GridPosition, const FGridSettings& GridSettings)
{
	glm::vec2 LocalPosition = PositionLocalSpace(GridPosition, GridSettings);
	glm::vec2 LocalOrigin = GridCentreLocalSpace(GridSettings);
	return LocalPosition - LocalOrigin;
}


glm::vec2 GetTileCentreWorldPosition(const FGridPosition& GridPosition, const FGridSettings& GridSettings)
{
	glm::vec2 OffsetFromCentre = OffsetFromOriginLocalSpace(GridPosition, GridSettings);
	return GridSettings.Origin + OffsetFromCentre;
}


glm::mat4 GetTileCentreTransform(const FGridPosition& GridPosition, const FGridSettings& GridSettings)
{
	const glm::vec2 TilePos = GetTileCentreWorldPosition(GridPosition, GridSettings);
	return glm::translate(glm::mat4(1.0f), glm::vec3(TilePos, 0.0f))
		* glm::scale(glm::mat4(1.0f), { GridSettings.TileScale, GridSettings.TileScale, 1.0f });
}

FGridSettings DefaultGridSettings()
{
	FGridSettings DefaultGridSettings;
	DefaultGridSettings.Dimensions.NumRows = NumGridRows;
	DefaultGridSettings.Dimensions.NumCols = NumGridCols;
	DefaultGridSettings.TileScale = TileScale;
	return DefaultGridSettings;
}

