#pragma once
#include "Item.h"

class Mushroom : public Item
{
public:
	Mushroom(class Game* game);

	void UpdateActor(float deltaTime) override;

	void CollisionPlayer(class Actor* actor) override;

private:
	class MoveComponent* mc;
	Vector2 moveSpeed;

	const float mDOWN_SEGMENT_LENGTH = 33.0f;

};