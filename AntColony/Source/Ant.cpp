#include "Ant.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "Ai/AStar.h"
#include "Renderer/OffsetBuffer.h"

#include "AntStates.h"
#include "AntWorld.h"
#include "TileGrid.h"
#include "GameConfig.h"

FAnt::FAnt(FAntWorld& AntWorld, FSpriteTexture SpriteTexture, const FGridPosition& HomePosition) : 
	m_AntWorld(AntWorld),
	m_Sprite(std::move(SpriteTexture)),
	m_CurrentGridPosition(HomePosition),
	m_HomeGridPosition(HomePosition),
	m_WorldPosition(GetTileCentreWorldPosition(HomePosition, m_AntWorld.m_TileGrid.GetSettings()))
{
	m_StateMachine.ChangeState(*this, std::make_unique<FWanderState>());
}

void FAnt::Update(const float DeltaSeconds)
{
	if (!HasReachedTarget())
	{
		UpdateMovement(DeltaSeconds);
	}

	if (InAcceptanceRadiusOfTarget())
	{
		m_CurrentGridPosition = m_TargetGridPosition;
	}

	m_StateMachine.Update(*this);
}

void FAnt::SubmitVerticesToBuffer(const FTextureManager& TextureManager, FRenderVertexData& RenderVertexData) const
{
	AddVerticesToBuffer(TextureManager, RenderVertexData.Vertices);
	AddIndicesToBuffer(RenderVertexData.Indices);
}

void FAnt::AddVerticesToBuffer(const FTextureManager& TextureManager, std::vector<float>& OutBuffer) const
{
	m_Sprite.AddVerticesToBuffer(TextureManager, OutBuffer);
}

void FAnt::AddIndicesToBuffer(FOffsetBuffer& OutBuffer) const
{
	m_Sprite.AddIndicesToBuffer(OutBuffer);
}

void FAnt::SetTargetGridPosition(const FGridPosition& GridPosition)
{
	m_TargetGridPosition = GridPosition;
	const FTileGrid& TileGrid = m_AntWorld.m_TileGrid;
	const FGridSettings& GridSettings = TileGrid.GetSettings();
	m_TargetWorldPosition = GetTileCentreWorldPosition(m_TargetGridPosition, GridSettings);
}

bool FAnt::InAcceptanceRadiusOfTarget() const
{
	const FTileGrid& TileGrid = m_AntWorld.m_TileGrid;
	const FGridSettings& GridSettings = TileGrid.GetSettings();
	constexpr float TileAcceptanceFraction = 0.1f;
	const float AcceptanceRadius = GridSettings.TileScale * TileAcceptanceFraction;
	const float AcceptanceRadiusSquared = AcceptanceRadius * AcceptanceRadius;
	const float DistanceToTargetSquared = glm::distance2(m_TargetWorldPosition, m_WorldPosition);
	return DistanceToTargetSquared < AcceptanceRadiusSquared;
}

void FAnt::UpdateMovement(const float DeltaSeconds)
{
	constexpr glm::vec2 ForwardVector = glm::vec2(0.0f, 1.0f);
	constexpr glm::vec3 UpVector = glm::vec3(0.0f, 0.0f, 1.0f);
	constexpr float SceneDepth = -0.1f;

	const glm::vec2 UnitDirectionToTarget = glm::normalize(m_TargetWorldPosition - m_WorldPosition);

	const float RotationAngle = glm::orientedAngle(ForwardVector, UnitDirectionToTarget);
	glm::mat4 Rotation = glm::rotate(glm::mat4(1.0f), RotationAngle, UpVector);

	m_WorldPosition += m_TilesPerSecond * UnitDirectionToTarget * DeltaSeconds;
	glm::mat4 Translation = glm::translate(glm::mat4(1.0f), glm::vec3(m_WorldPosition, SceneDepth)); 
	
	const float ScaleFactor = m_AntWorld.m_TileGrid.GetSettings().TileScale * AntToTileSizeRatio;
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), { ScaleFactor , ScaleFactor, 1.0f });
	
	m_Sprite.SetTransform(Translation * Rotation * Scale);
}
