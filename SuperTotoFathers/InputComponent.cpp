#include "InputComponent.h"
#include "Actor.h"
#include "Game.h"
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
	if (GetOwner()->GetGame()->GetScene() == Game::Scene::EMain)
	{
		if (!((mOwner->GetActionState() == Actor::ActionState::EDeath) || (mOwner->GetActionState() == Actor::ActionState::EGoal)))
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
					SetJumpPower(530.0f);
					GetOwner()->SetActionState(Actor::ActionState::EJump);
				}
				SetPrevJumpKey(true);
			}
			else
			{
				SetPrevJumpKey(false);
			}
		}

	}
	else if(GetOwner()->GetGame()->GetScene() == Game::Scene::EEnd)
	{
		if (GetOwner()->GetGame()->GetEngingCount() < 66)
		{
			if (GetOwner()->GetGame()->GetEngingCount() < 3)
			{
				SetIsPushJumpKey(true);
				SetCanJump(false);
				SetJumpPower(30.0f);
				GetOwner()->SetActionState(Actor::ActionState::EJump);
			}
			float moveForce = 80.0f;
			Vector2 force = Vector2(moveForce, 0.0f);
			AddForce(force);
		}
	}
	
}
