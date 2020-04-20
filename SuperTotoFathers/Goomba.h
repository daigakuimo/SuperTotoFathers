#pragma once
#include "Actor.h"

class Goomba : public Actor
{
public:
	Goomba(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
	

private:
	class MoveComponent* mc;
	Vector2 moveSpeed;

	class BoxComponent* mBoxComp;

	class CircleComponent* mCircleComp;

	std::vector<class CircleComponent*> mCircleList;

	const float mDOWN_SEGMENT_LENGTH = 39.0f;

	int deathCount = 0;
};