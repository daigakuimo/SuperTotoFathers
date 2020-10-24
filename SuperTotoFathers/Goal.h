#pragma once
#include "Actor.h"
#include "SoundEvent.h"

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

	class AudioComponent* mAudioComp;
	SoundEvent mSoundGoal;

	bool mFirstTouch = false;
};