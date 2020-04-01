#include "PlayerStateComponent.h"
#include "PlayerState.h"
#include "Actor.h"
#include <SDL_log.h>

PlayerStateComponent::PlayerStateComponent(class Actor* owner)
	:Component(owner)
	, mCurrentState(nullptr)
{

}

void PlayerStateComponent::Update(float deltaTime)
{
	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}

void PlayerStateComponent::ChangeState(const std::string& name)
{
	// First exit the current state
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	// Try to find the new state from the map
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mCurrentState = iter->second;
		// We're entering the new state
		mCurrentState->OnEnter();
	}
	else
	{
		mCurrentState = nullptr;
	}
}

void PlayerStateComponent::RegisterState(PlayerState* state)
{
	mStateMap.emplace(state->GetName(), state);
}