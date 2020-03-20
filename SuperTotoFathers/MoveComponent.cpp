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
	if (mIsGravity)
	{
		AddForce(mGrabityPower);
	}


	if (!Math::NearZero(mForwardSpeed.Length()) || !Math::NearZero(mSumOfForces.Length()))
	{

		Vector2 pos = mOwner->GetPosition();

		// udate acceleration from amount of force
		mForwardAcceleration = mSumOfForces * (1.0f / mMass);

		// update speed
		mForwardSpeed += mForwardAcceleration * deltaTime;

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

		if (pos.y < -352.0f) { pos.y = -352.0f; }
		else if (pos.y > 352.0f) { pos.y = 352.0f; }

		mOwner->SetPosition(pos);
	}
}

void MoveComponent::AddForce(Vector2& force)
{
	mSumOfForces += force;
}
