#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent
{
public:
	// Lower update order to update first
	InputComponent(class Actor* owner);

	void ProcessInput(const uint8_t* keyState) override;

	// Getters/Setters for private variables
	int   GetForwardKey()          const { return mForwardKey; }
	int   GetBackKey()             const { return mBackKey; }
	int   GetJumpKey()             const { return mJumpKey; }

	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackKey(int key) { mBackKey = key; }
	void SetJumpKey(int key) { mJumpKey = key; }
	void SetDashKey(int key) { mDashKey = key; }
	void SetPrevJumpKey(bool pushKey) { mIsPrevJumpKey = pushKey; }

private:
	// Keys for forward/back movement
	int mForwardKey;
	int mBackKey;

	// keys for jump movement
	int mJumpKey;

	// keys for dash
	int mDashKey;

	// Previous push jump key
	bool mIsPrevJumpKey;
};