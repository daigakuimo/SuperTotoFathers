#include "CircleComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"

CircleComponent::CircleComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mObjectCircle(Vector3::Zero, 0.f)
	, mWorldCircle(Vector3::Zero, 0.0f)
{

}

CircleComponent::~CircleComponent()
{

}

void CircleComponent::OnUpdateWorldTransform()
{
	mWorldCircle.mCenter = Vector3(mOwner->GetPosition().x, mOwner->GetPosition().y, 0.0f);

	mWorldCircle.mRadius = mObjectCircle.mRadius;

	mWorldCircle.mRadius *= mOwner->GetScale();
}