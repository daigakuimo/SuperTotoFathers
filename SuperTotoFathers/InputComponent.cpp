#include "InputComponent.h"
#include "Actor.h"
#include <SDL_Log.h>

InputComponent::InputComponent(class Actor* owner)
	:MoveComponent(owner)
	, mForwardKey(0)
	, mBackKey(0)
	, mJumpKey(0)
	, mDashKey(0)
	, mIsPrevJumpKey(false)
{
}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	// Calculate forward speed for MoveComponent
	float moveForce = 80.0f;
	if (keyState[mDashKey])
	{
		moveForce = 120.0f;
	}

	Vector2 force = Vector2::Zero;
	if (keyState[mForwardKey])
	{
		force = Vector2(moveForce, 0.0f);
	}
	if (keyState[mBackKey])
	{
		force = Vector2(-moveForce, 0.0f);
	}
	AddForce(force);
	//SetForwardSpeed(force);

	// Calculate jump 
	SetIsPushJumpKey(false);
	if (keyState[mJumpKey])
	{
		if (GetCanJump() && !mIsPrevJumpKey)
		{
			SetIsPushJumpKey(true);
			SetCanJump(false);
			SetJumpPower(250.0f);
			/*SDL_Log("ccc");
			SDL_Log("");*/
		}
		SetPrevJumpKey(true);
	}
	else
	{
		SetPrevJumpKey(false);
	}
}
