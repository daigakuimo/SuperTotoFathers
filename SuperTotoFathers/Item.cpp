#include "Item.h"
#include "Game.h"
#include "BoxComponent.h"

Item::Item(class Game* game)
	:Actor(game)
	,mBoxComp(nullptr)
{

}

Item::~Item()
{
	// delete from Item List
	GetGame()->RemoveItem(this);
}

void Item::CollisionPlayer(class Actor* actor)
{

}

void Item::SetBoxComp(const AABB &a)
{
	mBoxComp = new BoxComponent(this);

	mBoxComp->SetObjectBox(a);
	mBoxComp->SetShouldRotate(false);
}
