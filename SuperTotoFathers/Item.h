#pragma once
#include "Actor.h"
#include "Collision.h"

class Item : public Actor
{
public:
	Item(class Game* game);
	~Item();

	void UpdateActor(float deltaTime) override {}

	virtual void CollisionPlayer(class Actor* actor);

	class BoxComponent* GetBoxComp() { return mBoxComp; }
	void SetBoxComp(const AABB &a);

private:

	class BoxComponent* mBoxComp;

};