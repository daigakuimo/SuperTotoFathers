#pragma once
#include "Actor.h"

class GoalFlag : public Actor
{
public:
	GoalFlag(class Game* game);

	void UpdateActor(float deltaTime) override;

	void FirstFlagPosY() { mFirstFlagPosY = GetPosition().y; }
	float GetFirstFlagPosY() { return mFirstFlagPosY; }

private:
	class MoveComponent* mMoveComp;
	float mFirstFlagPosY;
};