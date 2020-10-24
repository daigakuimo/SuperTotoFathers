#include "GoalFlag.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Texture.h"

GoalFlag::GoalFlag(class Game* game)
	:Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/flag.png"));

	mMoveComp = new MoveComponent(this);
	mMoveComp->SetMass(1.0f);
	mMoveComp->SetVelocityLimit(Vector2(20.0f, 200.0f));


}

void GoalFlag::UpdateActor(float deltaTime)
{
	if(GetActionState() == ActionState::EWalk)
	{ 
		// ƒS[ƒ‹ƒ|[ƒ‹‚Ìˆê”Ô‚µ‚½‚Ü‚Å—‚¿‚é
		if (GetFirstFlagPosY() - GetPosition().y <= 8.0f * 64.0f)
		{
			Vector2 force = Vector2(0.0f, -200.0f);
			mMoveComp->AddForce(force);
		}
		else
		{
			SetPosition(Vector2(GetPosition().x, GetFirstFlagPosY() - 8.0f * 64.0f));
			if (GetGame()->GetScene() == Game::Scene::EMain)
			{
				GetGame()->ChangeSceneToEnd();
				GetGame()->StartEndingBGM();
			}
			
		}
	}
}