#pragma once

#include "glm/glm.hpp"

#include "Ai/AStar.h"
#include "Ai/FiniteStateMachine.h"

#include "Ant.h"

class FTile;
class FTileGrid;

class FWanderState : public FState<FAnt>
{
public:
	virtual void OnEnter(FAnt& Entity) override;
	virtual void OnExit(FAnt& Entity) override;
	virtual void OnUpdate(FAnt& Entity, FFiniteStateMachine<FAnt>& Fsm) override;
	
private:
	void SelectRandomDirection();
	bool CanSenseFoodSource(const FGridPosition& CurrentPosition, const FGridPosition& FoodPosition) const;
	bool InHistory(const FGridPosition& CurrentPosition) const;

	FGridPosition SelectTargetGridCell(const FTileGrid& TileGrid, const FGridPosition& SourcePosition);
	const FTile* GetTileInPreferredDirection(const FTileGrid& TileGrid, const FGridPosition& SourcePosition) const;
	const FTile* FindNeighbourWithHigherPheromoneLevel(const FTileGrid& TileGrid, const FGridPosition& SourcePosition, const float PheromoneLevel) const;
private:
	//TODO: Make Cyclic Buffer
	static constexpr std::size_t s_BufferSize = 3;
	FGridPositions m_History = FGridPositions(s_BufferSize);
	std::size_t m_LastEntry = 0;

	glm::i8vec2 m_PreferredDirection = glm::i8vec2(0);
	FGridPosition m_TargetGridPosition;
};

class FPathFollowingState : public FState<FAnt>
{
public:
	FPathFollowingState(FAnt& Entity, FGridPositions&& Path) { SetPath(Entity, std::move(Path)); }
public:
	virtual void OnEnter(FAnt& Entity) override;
	virtual void OnExit(FAnt& Entity) override;
	virtual void OnUpdate(FAnt& Entity, FFiniteStateMachine<FAnt>& Fsm) override;

protected:
	void SetPath(FAnt& Entity, FGridPositions&& Path);
	bool IsPathComplete() const;

protected:
	FGridPositions m_Path;
	std::size_t m_CurrentPathIndex = 0;
};

class FCollectFoodState : public FPathFollowingState
{
public:
	FCollectFoodState(FAnt& Entity, FGridPositions&& Path);

public:
	virtual void OnEnter(FAnt& Entity) override;
	virtual void OnExit(FAnt& Entity) override;
	virtual void OnUpdate(FAnt& Entity, FFiniteStateMachine<FAnt>& Fsm) override;
};

class FTransportFoodToNestState : public FPathFollowingState
{
public:
	FTransportFoodToNestState(FAnt& Entity, FGridPositions&& Path);

public:
	virtual void OnEnter(FAnt& Entity) override;
	virtual void OnExit(FAnt& Entity) override;
	virtual void OnUpdate(FAnt& Entity, FFiniteStateMachine<FAnt>& Fsm) override;
};