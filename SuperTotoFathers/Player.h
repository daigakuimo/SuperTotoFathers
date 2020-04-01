#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;

private:
	// constant of action
	const int mACTION_STOP = 0;
	const int mACTION_WALK = 1;

	class InputComponent* ic;
};