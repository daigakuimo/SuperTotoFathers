#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	// Lower update order to update first
	MoveComponent(class Actor* owner, float mass, Vector2 velocityLimit, bool isGravity = true,  int updateOrder = 10);

	void Update(float deltaTime) override;

	void AddForce(Vector2& force);

	float GetAngularSpeed() const { return mAngularSpeed; }
	void  SetAngularSpeed(float speed) { mAngularSpeed = speed; }

	Vector2 GetForwardSpeed() const { return mForwardSpeed; }
	void  SetForwardSpeed(Vector2 speed) { mForwardSpeed = speed; }

	Vector2 GetSumOfForce() const { return mSumOfForces; }
	void SetSumOfForce(Vector2 sumOfForce) { mSumOfForces = sumOfForce; }

	void SetIsPushJumpKey(bool pushKey) { mIsPushJumpKey = pushKey; }
	void SetJumpPower(float jumpPower) { mJumpPower = jumpPower; }
	bool GetCanJump() const { return mCanJump; }
	void SetMaxJumpHeight(float height) { mMaxJumpHeight = height; }
	float GetMaxJump() const { return mMaxJumpHeight; }


private:
	// Controls rotation (radians/second)
	float mAngularSpeed;

	// Controls forward movement (units/second)
	Vector2 mForwardSpeed;

	// Previous actor position
	Vector2 mPrevPosition;

	// mass
	float mMass;

	// is gravity
	bool mIsGravity;

	// gravity
	Vector2 mGrabityPower = Vector2(0.0f, -200.0f);

	// amount of force
	Vector2 mSumOfForces;

	// speed
	Vector2 mVelocity;

	// speed limit
	Vector2 mVelocityLimit;

	// accleration
	Vector2 mForwardAcceleration;

	// The maximum jump height
	float mMaxJumpHeight;

	// can jump flag
	bool mCanJump;

	// now jumping flag
	bool mIsJumping;

	// push jump key
	bool mIsPushJumpKey;

	// Jump power
	float mJumpPower;


};