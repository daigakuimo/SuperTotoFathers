#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>

class PlayerStateComponent : public Component
{
public:
	PlayerStateComponent(class Actor* owner);

	void Update(float deltaTime) override;
	void ChangeState(const std::string& name);

	// Add a new state to the map
	void RegisterState(class PlayerState* state);

private:
	// Maps name of state to PlayerState instance
	std::unordered_map<std::string, class PlayerState*> mStateMap;

	// Current state we're in
	class PlayerState* mCurrentState;
};