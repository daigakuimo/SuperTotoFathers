#pragma once
#include "Actor.h"

class Brock : public Actor
{
public:
	enum ContainItem
	{
		ENone,
		ECoin,
		EStar,
		EHatenaCoin,
		EHatenaMushroom
	};

	Brock(class Game* game, int item);
	~Brock();

	void UpdateActor(float deltaTime) override;

	ContainItem GetContainItem() { return mContainItem; }
	void SetContainItem(ContainItem item) { mContainItem = item; }

	class BoxComponent* GetBoxComp() { return mBoxComp; }

private:

	ContainItem mContainItem;

	class BoxComponent* mBoxComp;

};