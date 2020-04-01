#pragma once
#include "Actor.h"

class CameraActor : public Actor
{
public:
	CameraActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void SetActor(class Actor* followActor) { mFollowActor = followActor; mBeforeX = mFollowActor->GetPosition().x; }

	float GetBeforeX() { return mBeforeX; }
private:
	class MoveComponent* mMoveComp;
	class Actor* mFollowActor;

	float mBeforeX;
};