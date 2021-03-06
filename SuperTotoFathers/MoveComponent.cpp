#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"
#include <SDL_log.h>
#include "CameraActor.h"


int count = 0;

MoveComponent::MoveComponent(class Actor* owner,int updateOrder)
	:Component(owner, updateOrder)
	,mAngularSpeed(0.f)
	,mForwardSpeed(Vector2::Zero)
	,mMass(0.f)
	,mVelocityLimit(0.f,0.f)
	,timeCount(0)
	,mIsGravity(false)
{
}

void MoveComponent::Update(float deltaTime)
{

	Vector2 pos = mOwner->GetPosition();

	Actor::ActionState currentAction = mOwner->GetActionState();

	if (mIsGravity && !(currentAction == Actor::ActionState::EStop || currentAction == Actor::ActionState::EWalk))
	{
		AddForce(mGrabityPower);
	}

	if (!Math::NearZero(mForwardSpeed.Length()) || !Math::NearZero(mSumOfForces.Length()) || !Math::NearZero(mJumpPower))
	{
		if (!Math::NearZero(mSumOfForces.Length()))
		{
			timeCount++;
			if (timeCount > 30)
			{
				// sinはPi/2の時に最大値１になるから30まで
				timeCount = 30;
			}
		}
		else
		{
			timeCount = 0;
		}

		// udate acceleration from amount of force
		mForwardAcceleration = mSumOfForces * (1.0f / mMass);

		// update speed
		mForwardSpeed += mForwardAcceleration * sin(Math::Pi * 1/60 * timeCount) * deltaTime;

		// jump action
		if (mIsPushJumpKey || mIsJumping || mJumpPower != 0)
		{
			if (mIsPushJumpKey)
			{
				mPrevPosition = pos;
			}

			float tempPosY = (pos.y + 384.0f) - (mPrevPosition.y + 384.0f);
			
			if (tempPosY >= mMaxJumpHeight && mJumpPower > 0)
			{
				mJumpPower = 0;
			}

			mForwardSpeed.y = mJumpPower;
			if (mJumpPower < 0)
			{
				mJumpPower -= 50;
			}
			else if (mJumpPower < 100)
			{
				mJumpPower -= 40;
			}
			else
			{
				mJumpPower -= 20;
			}
		}

		// limit speed
		if (mForwardSpeed.x > mVelocityLimit.x)
		{
			mForwardSpeed.x = mVelocityLimit.x;
		}
		if (mForwardSpeed.x < -mVelocityLimit.x)
		{
			mForwardSpeed.x = -mVelocityLimit.x;
		}
		if (mForwardSpeed.y > mVelocityLimit.y)
		{
			mForwardSpeed.y = mVelocityLimit.y;
		}
		if (mForwardSpeed.y < -mVelocityLimit.y)
		{
			mForwardSpeed.y = -mVelocityLimit.y;
		}

		// case stop
		if (!mIsJumping)
		{
			if (mForwardSpeed.x > 0 && mSumOfForces.x == 0)
			{
				mForwardSpeed.x *= 0.93f;
				if (mForwardSpeed.x <= 30)
				{
					mForwardSpeed.x = 0;
				}
			}
			else if (mForwardSpeed.x < 0 && mSumOfForces.x == 0)
			{
				mForwardSpeed.x *= 0.93f;
				if (mForwardSpeed.x >= -30)
				{
					mForwardSpeed.x = 0;
				}
			}
		}
		else
		{
			if (mForwardSpeed.x > 0 && mSumOfForces.x == 0)
			{
				mForwardSpeed.x *= 0.96f;
			}
			else if (mForwardSpeed.x < 0 && mSumOfForces.x == 0)
			{
				mForwardSpeed.x *= 0.96f;
			}
		}

		// case return
		if (mForwardSpeed.x > 0 && mSumOfForces.x < 0)
		{
			mForwardSpeed.x *= 0.92f;
		}
		if (mForwardSpeed.x < 0 && mSumOfForces.x > 0)
		{
			mForwardSpeed.x *= 0.92f;
		}

		mSumOfForces = Vector2::Zero;

		// pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		pos += mForwardSpeed * deltaTime;
		
		if (currentAction == Actor::ActionState::EStop || currentAction == Actor::ActionState::EWalk)
		{ 
			mJumpPower = 0;
			mIsJumping = false;
			mCanJump = true;
		}

		mOwner->SetPosition(pos);
	}
}

void MoveComponent::AddForce(Vector2& force)
{
	mSumOfForces += force;
}


