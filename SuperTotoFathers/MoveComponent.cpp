#include "MoveComponent.h"
#include "Actor.h"
#include <SDL_log.h>

int count = 0;

MoveComponent::MoveComponent(class Actor* owner, float mass, Vector2 velocityLimit, bool isGravity, int updateOrder)
	:Component(owner, updateOrder)
	,mAngularSpeed(0.0f)
	,mForwardSpeed(Vector2::Zero)
	,mMass(mass)
	,mVelocityLimit(velocityLimit)
	, mIsGravity(isGravity)
{
}

void MoveComponent::Update(float deltaTime)
{

	Vector2 pos = mOwner->GetPosition();

	if (mIsGravity)
	{
		AddForce(mGrabityPower);
	}


	if (!Math::NearZero(mForwardSpeed.Length()) || !Math::NearZero(mSumOfForces.Length()) || !Math::NearZero(mJumpPower))
	{

		// udate acceleration from amount of force
		mForwardAcceleration = mSumOfForces * (1.0f / mMass);

		// update speed
		mForwardSpeed += mForwardAcceleration * deltaTime;

		// jump action
		if (mIsPushJumpKey || mIsJumping)
		{
			if (mIsPushJumpKey)
			{
				mPrevPosition = pos;
				mIsJumping = true;
				mCanJump = false;
			}

			float tempPosY = (pos.y + 384.0f) - (mPrevPosition.y + 384.0f);

			SDL_Log("jump : %3.3f : %3.3f", tempPosY, mMaxJumpHeight);
			

			if (tempPosY >= mMaxJumpHeight && mJumpPower > 0)
			{
				mJumpPower = 0;
				SDL_Log("aaa");
			}

			mForwardSpeed.y = mJumpPower;
			mJumpPower -= 5;
			if (mJumpPower < 0)
			{
				mJumpPower -= 6;
			}
			if (mJumpPower < 40)
			{
				mJumpPower -= 5;
			}
			// Todo : XŽ²ˆÚ“®‚µ‚·‚¬‚â‚©‚ç•ÏX‚·‚é
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


		// Adjust inertia

		// case stop
		if (!mIsJumping)
		{
			if (mForwardSpeed.x > 0 && mSumOfForces.x == 0)
			{
				mForwardSpeed.x *= 0.95f;
				if (mForwardSpeed.x <= 10)
				{
					mForwardSpeed.x = 0;
				}
			}
			else if (mForwardSpeed.x < 0 && mSumOfForces.x == 0)
			{
				mForwardSpeed.x *= 0.95f;
				if (mForwardSpeed.x >= -10)
				{
					mForwardSpeed.x = 0;
				}
			}
		}

		// case return
		if (mForwardSpeed.x > 0 && mSumOfForces.x < 0)
		{
			mForwardSpeed.x *= 0.95f;
		}
		if (mForwardSpeed.x < 0 && mSumOfForces.x > 0)
		{
			mForwardSpeed.x *= 0.95f;
		}

		mSumOfForces = Vector2::Zero;

		// pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		pos += mForwardSpeed * deltaTime;

		// (Screen wrapping code only for asteroids)
		if (pos.x < -512.0f) { pos.x = 510.0f; }
		else if (pos.x > 512.0f) { pos.x = -510.0f; }

		if (pos.y < -224.0f) 
		{ 
			pos.y = -224.0f; 
			mJumpPower = 0;
			mIsJumping = false;
			mCanJump = true;
		}
		else if (pos.y > 352.0f) { pos.y = 352.0f; }

		mOwner->SetPosition(pos);
	}
}

void MoveComponent::AddForce(Vector2& force)
{
	mSumOfForces += force;
}
