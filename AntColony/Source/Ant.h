#pragma once

#include "glm/glm.hpp"

#include "Ai/FiniteStateMachine.h"
#include "Renderer/Sprite.h"

#include "GridPosition.h"

class FAntWorld;
class FSprite;
class FTextureManager;
struct FRenderVertexData;

class FAnt
{
public:
	FAnt(FAntWorld& AntWorld, FSpriteTexture SpriteTexture, const FGridPosition& Position);

public:
	void Update(const float DeltaSeconds);
	void SubmitVerticesToBuffer(const FTextureManager& TextureManager, FRenderVertexData& RenderVertexData) const;

public:
	FAntWorld& GetWorld() { return m_AntWorld; };
	glm::vec2 GetWorldPosition() const { return m_WorldPosition; };
	FGridPosition GetGridPosition() const { return m_CurrentGridPosition; };
	FGridPosition GetHomeGridPosition() const { return m_HomeGridPosition; };

	void SetTargetGridPosition(const FGridPosition& GridPosition);
	bool HasReachedTarget() const { return m_TargetGridPosition == m_CurrentGridPosition; }

private:
	bool InAcceptanceRadiusOfTarget() const;
	void UpdateMovement(const float DeltaSeconds);

	void AddVerticesToBuffer(const FTextureManager& TextureManager, std::vector<float>& OutBuffer) const;
	void AddIndicesToBuffer(FOffsetBuffer& OutBuffer) const;

private:
	FAntWorld& m_AntWorld;
	FSprite m_Sprite;

	FFiniteStateMachine<FAnt> m_StateMachine;

	FGridPosition m_HomeGridPosition;
	FGridPosition m_CurrentGridPosition;
	FGridPosition m_TargetGridPosition; 

	glm::vec2 m_WorldPosition;
	glm::vec2 m_TargetWorldPosition;
	float m_TilesPerSecond = 0.25f;
};