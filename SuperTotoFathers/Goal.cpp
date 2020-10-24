#include "Goal.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "BoxComponent.h"
#include "Texture.h"
#include "GoalFlag.h"
#include "AudioSystem.h"
#include "AudioComponent.h"

Goal::Goal(Game* game)
	:Actor(game)
{
	mBoxComp = new BoxComponent(this);

	AABB myBox(Vector3(-4.0f, -304.0f, 0.f),
		Vector3(4.0f, 304.0f, 0.f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/goal.png"));

	mAudioComp = new AudioComponent(this);
	mSoundGoal = mAudioComp->PlayEvent("event:/goalflag");
	mSoundGoal.SetPaused(true);
}

void Goal::UpdateActor(float deltaTime)
{
	if (GetActionState() == ActionState::EWalk)
	{
		mFlag->SetActionState(ActionState::EWalk);
		if (!mFirstTouch)
		{
			mSoundGoal.SetPaused(false);
			mSoundGoal.Restart();
			mFirstTouch = true;
		}
		
	}
}

void Goal::CreateFlag()
{
	// ゴールの旗のインスタン作成
	mFlag = new GoalFlag(GetGame());
	mFlag->SetPosition(GetPosition() + Vector2(-32.0f, 224.0f));
	mFlag->FirstFlagPosY();
}