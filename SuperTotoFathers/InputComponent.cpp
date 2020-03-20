#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner, float mass, Vector2 velocityLimit, bool isGravity)
	:MoveComponent(owner, mass, velocityLimit, isGravity)
	, mForwardKey(0)
	, mBackKey(0)
	, mClockwiseKey(0)
	, mCounterClockwiseKey(0)
{

}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	// Calculate forward speed for MoveComponent
	Vector2 force = Vector2::Zero;
	if (keyState[mForwardKey])
	{
		force = Vector2(100.0f, 0.0f);
	}
	if (keyState[mBackKey])
	{
		force = Vector2(-100.0f, 0.0f);
	}
	AddForce(force);
	//SetForwardSpeed(force);

	// Calculate angular speed for MoveComponent
	float angularSpeed = 0.0f;
	if (keyState[mClockwiseKey])
	{
		angularSpeed += mMaxAngularSpeed;
	}
	if (keyState[mCounterClockwiseKey])
	{
		angularSpeed -= mMaxAngularSpeed;
	}
	SetAngularSpeed(angularSpeed);
}
