#pragma once
#include "Actor.h"

class Goal : public Actor
{
public:
	Goal(class Game* game);

	void UpdateActor(float deltaTime) override;

	class BoxComponent* GetBoxComp() { return mBoxComp; }

	void CreateFlag();

private:
	class BoxComponent* mBoxComp;
	class GoalFlag* mFlag;
};