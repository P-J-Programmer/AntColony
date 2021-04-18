#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "GridPosition.h"

struct FGridDimensions
{
	int NumRows = 0;
	int NumCols = 0;
};

struct FGridSettings
{
	glm::vec2 Origin = glm::vec2(0.0f);
	FGridDimensions Dimensions;
	float TileScale = 1.0f;
};

glm::vec2 PositionLocalSpace(const FGridPosition& GridPosition, const FGridSettings& GridSettings);
glm::vec2 GridCentreLocalSpace(const FGridSettings& GridSettings);
glm::vec2 OffsetFromOriginLocalSpace(const FGridPosition& GridPosition, const FGridSettings& GridSettings);
glm::vec2 GetTileCentreWorldPosition(const FGridPosition& GridPosition, const FGridSettings& GridSettings);
glm::mat4 GetTileCentreTransform(const FGridPosition& GridPosition, const FGridSettings& GridSettings);

FGridSettings DefaultGridSettings();

