#pragma once
#include "Actor.h"
#include "SoundEvent.h"

class Player : public Actor
{
public:
	Player(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;

	void SetFootstepSurface(float value);

private:
	const float mDOWN_SEGMENT_LENGTH = 32.0f;

	class InputComponent* ic;
	class BoxComponent* mBoxComp;

	Vector2 mPrevPos;

	class AudioComponent* mAudioComp;
	// SoundEvent mFootstep;
	// float mLastFootstep;

	SoundEvent mSoundDeath;
};