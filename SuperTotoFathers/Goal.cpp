#include "Goal.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "BoxComponent.h"
#include "Texture.h"
#include "GoalFlag.h"

Goal::Goal(Game* game)
	:Actor(game)
{
	mBoxComp = new BoxComponent(this);

	AABB myBox(Vector3(-4.0f, -304.0f, 0.f),
		Vector3(4.0f, 304.0f, 0.f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("../Assets/goal.png"));
}

void Goal::UpdateActor(float deltaTime)
{
	if (GetActionState() == ActionState::EWalk)
	{
		mFlag->SetActionState(ActionState::EWalk);
	}
}

void Goal::CreateFlag()
{
	// �S�[���̊��̃C���X�^���쐬
	mFlag = new GoalFlag(GetGame());
	mFlag->SetPosition(GetPosition() + Vector2(-32.0f, 224.0f));
	mFlag->FirstFlagPosY();
}