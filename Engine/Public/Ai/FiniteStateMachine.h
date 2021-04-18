#pragma once

#include <memory>

template <typename EntityType>
class FFiniteStateMachine;

template <typename EntityType>
class FState
{
public:
	virtual void OnEnter(EntityType& Entity) = 0;
	virtual void OnExit(EntityType& Entity) = 0;
	virtual void OnUpdate(EntityType& Entity, FFiniteStateMachine<EntityType>& StateMachine) = 0;
};


template <typename EntityType>
class FFiniteStateMachine
{
public:
	void ChangeState(EntityType& Owner, std::unique_ptr<FState<EntityType>> NewState)
	{
		if (m_CurrentState)
		{
			m_CurrentState->OnExit(Owner);
		}

		if (NewState)
		{
			NewState->OnEnter(Owner);
		}

		m_CurrentState = std::move(NewState);
	}

	void Update(EntityType& Owner)
	{
		if (m_CurrentState)
		{
			m_CurrentState->OnUpdate(Owner, *this);
		}
	}

private:
	std::unique_ptr<FState<EntityType>> m_CurrentState = nullptr;
};
