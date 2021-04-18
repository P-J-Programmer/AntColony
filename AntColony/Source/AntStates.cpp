#include "AntStates.h"

#include "Core/Random.h"

#include "AntWorld.h"
#include "FoodManager.h"
#include "GameConfig.h"
#include "TileGrid.h"

void FWanderState::OnEnter(FAnt& Entity)
{
	SelectRandomDirection();
	const FGridPosition Target = SelectTargetGridCell(Entity.GetWorld().m_TileGrid, Entity.GetGridPosition());
	Entity.SetTargetGridPosition(Target);
}

void FWanderState::OnExit(FAnt& Entity)
{
}

void FWanderState::OnUpdate(FAnt& Entity, FFiniteStateMachine<FAnt>& StateMachine)
{
	const FGridPosition CurrentPosition = Entity.GetGridPosition();

	const FGridPosition FoodPosition = Entity.GetWorld().m_FoodManager.ClosestFoodSource(CurrentPosition);
	if (CanSenseFoodSource(CurrentPosition, FoodPosition))
	{
		AStar<FGridPosition>::FPathfindingResult PathResult = AStar<FGridPosition>::Search(Entity.GetWorld().m_TileGrid, CurrentPosition, FoodPosition);
		StateMachine.ChangeState(Entity, std::make_unique<FCollectFoodState>(Entity, std::move(PathResult.Path)));
		return;
	}

	if (Entity.HasReachedTarget())
	{
		const FGridPosition Target = SelectTargetGridCell(Entity.GetWorld().m_TileGrid, CurrentPosition);
		Entity.SetTargetGridPosition(Target);
	}
}

void FWanderState::SelectRandomDirection()
{
	m_PreferredDirection = glm::i8vec2(0);
	while (m_PreferredDirection == glm::i8vec2(0))
	{
		m_PreferredDirection.x = Random::RandInRange(-1, 1);
		m_PreferredDirection.y = Random::RandInRange(-1, 1);
	}
}

FGridPosition FWanderState::SelectTargetGridCell(const FTileGrid& TileGrid, const FGridPosition& SourcePosition)
{
	/* Take tile in the preferred direction (where possible) unless one of the  neighbours has a higher pheromone count. */
	const FTile* NextTile = GetTileInPreferredDirection(TileGrid, SourcePosition);	

	/* Tile in the preferred direction is not available, try a different direction next time */
	if (NextTile == nullptr)
	{
		SelectRandomDirection();
	}

	const float PheromoneLevel = NextTile ? NextTile->GetPheromoneLevel(EPheromoneType::Attractive) : -1;
	const FTile* BetterNeighbour = FindNeighbourWithHigherPheromoneLevel(TileGrid, SourcePosition, PheromoneLevel);
	if (BetterNeighbour != nullptr)
	{
		NextTile = BetterNeighbour;
	}

	/* Failed to find any possible tile */
	if (NextTile == nullptr)
	{
		return SourcePosition;
	}

	const FGridPosition NextPosition = TileGrid.GetPositionOfTile(NextTile);
	m_LastEntry = (m_LastEntry + 1) % s_BufferSize;
	m_History[m_LastEntry] = NextPosition;
	return NextPosition;
}

const FTile* FWanderState::GetTileInPreferredDirection(const FTileGrid& TileGrid, const FGridPosition& SourcePosition) const
{
	FGridPosition NextPosition(SourcePosition);
	NextPosition.Row += m_PreferredDirection.x;
	NextPosition.Col += m_PreferredDirection.y;

	if (TileGrid.IsValidPosition(NextPosition) && !InHistory(NextPosition))
	{
		const FTile* const PreferredTile = TileGrid.GetTileAtPosition(NextPosition); // cache
		if (PreferredTile && PreferredTile->IsNavigable())
		{
			return PreferredTile;
		}
	}

	return nullptr;
}

const FTile* FWanderState::FindNeighbourWithHigherPheromoneLevel(const FTileGrid& TileGrid, const FGridPosition& SourcePosition, const float PheromoneLevel) const
{
	const FTile* HighestScoringNeighbour = nullptr;
	float HighestPheromoneLevel = PheromoneLevel;

	std::vector<const FTile*> Neighbours = TileGrid.GetNavigableNeighbours(SourcePosition, ENeighbourMode::Moore);
	for (const FTile* Neighbour : Neighbours)
	{
		/* Ignore recently visited neighbours */
		if (InHistory(TileGrid.GetPositionOfTile(Neighbour)))
		{
			continue;
		}

		const float NeighbourPheromoneLevel = Neighbour->GetPheromoneLevel(EPheromoneType::Attractive);
		if (NeighbourPheromoneLevel > HighestPheromoneLevel)
		{
			HighestPheromoneLevel = PheromoneLevel;
			HighestScoringNeighbour = Neighbour;
		}
	}

	return HighestScoringNeighbour;
}

bool FWanderState::CanSenseFoodSource(const FGridPosition& CurrentPosition, const FGridPosition& FoodPosition) const
{
	constexpr int FoodDetectionRange = 3;
	return (ManhattanDistance(CurrentPosition, FoodPosition) <= FoodDetectionRange);
}

bool FWanderState::InHistory(const FGridPosition& Position) const
{
	return (std::find(m_History.begin(), m_History.end(), Position) != m_History.end());
}

/*
 ****************************************************************************
 ****************************************************************************
 */

void FPathFollowingState::OnEnter(FAnt& Entity)
{

}

void FPathFollowingState::OnExit(FAnt& Entity)
{
}

void FPathFollowingState::OnUpdate(FAnt& Entity, FFiniteStateMachine<FAnt>& StateMachine)
{
	if (!Entity.HasReachedTarget())
	{
		return;
	}

	++m_CurrentPathIndex;
	if (!IsPathComplete())
	{
		Entity.SetTargetGridPosition(m_Path[m_CurrentPathIndex]);
	}
}

void FPathFollowingState::SetPath(FAnt& Entity, FGridPositions&& Path)
{
	m_Path = std::move(Path);
	m_CurrentPathIndex = 0;
	if (!IsPathComplete())
	{
		Entity.SetTargetGridPosition(m_Path.front());
	}
}

bool FPathFollowingState::IsPathComplete() const
{
	return m_CurrentPathIndex >= m_Path.size();
}

/*
 ****************************************************************************
 **************************************************************************** 
 */

FCollectFoodState::FCollectFoodState(FAnt& Entity, FGridPositions&& Path) :
	FPathFollowingState(Entity, std::move(Path))
{
}

void FCollectFoodState::OnEnter(FAnt& Entity)
{
	FPathFollowingState::OnEnter(Entity);
}

void FCollectFoodState::OnExit(FAnt& Entity)
{
	FPathFollowingState::OnExit(Entity);
}

void FCollectFoodState::OnUpdate(FAnt& Entity, FFiniteStateMachine<FAnt>& StateMachine)
{
	FPathFollowingState::OnUpdate(Entity, StateMachine);

	if (IsPathComplete())
	{
		constexpr int FoodCapacity = 1;
		Entity.GetWorld().m_FoodManager.ChangeFoodAmount(m_Path.back(), -FoodCapacity); 
		AStar<FGridPosition>::FPathfindingResult PathResult = AStar<FGridPosition>::Search(Entity.GetWorld().m_TileGrid, Entity.GetGridPosition(), Entity.GetHomeGridPosition());
		StateMachine.ChangeState(Entity, std::make_unique<FTransportFoodToNestState>(Entity, std::move(PathResult.Path)));
	}
}

/*
 ****************************************************************************
 ****************************************************************************
 */

FTransportFoodToNestState::FTransportFoodToNestState(FAnt& Entity, FGridPositions&& Path)
 : FPathFollowingState(Entity, std::move(Path))
{
}

void FTransportFoodToNestState::OnEnter(FAnt& Entity)
{
	FPathFollowingState::OnEnter(Entity);
}

void FTransportFoodToNestState::OnExit(FAnt& Entity)
{
	FPathFollowingState::OnExit(Entity);
}

void FTransportFoodToNestState::OnUpdate(FAnt& Entity, FFiniteStateMachine<FAnt>& StateMachine)
{
	if (Entity.HasReachedTarget())
	{
		Entity.GetWorld().m_TileGrid.ModifyPheromoneLevel(Entity.GetGridPosition(), EPheromoneType::Attractive, PheromoneDepositSize); 
	}

	FPathFollowingState::OnUpdate(Entity, StateMachine);

	if (IsPathComplete())
	{
		StateMachine.ChangeState(Entity, std::make_unique<FWanderState>());
	}
}

/*
 ****************************************************************************
 ****************************************************************************
 */
