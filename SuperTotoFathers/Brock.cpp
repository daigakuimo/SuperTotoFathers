#include "Brock.h"
#include "SpriteComponent.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include "Texture.h"
#include "BoxComponent.h"

Brock::Brock(class Game* game, int item)
	:Actor(game)
{
	mContainItem = static_cast<ContainItem>(item);

	switch (mContainItem)
	{
		case ContainItem::EHatenaMushroom:
		case ContainItem::EHatenaCoin:
		{
			// Create an animation sprite compoent
			AnimSpriteComponent* asc = new AnimSpriteComponent(this);
			std::vector<class Texture*> anims = {
				game->GetTexture("../Assets/Hatena-1.png"),
				game->GetTexture("../Assets/Hatena-2.png"),
				game->GetTexture("../Assets/Hatena-3.png"),
				game->GetTexture("../Assets/Hatena-4.png"),
			};

			asc->SetAnimTextures(anims);
			asc->SetAnimRange(ActionState::EStop, 0, 2, true);
			asc->SetAnimRange(ActionState::EDeath, 3, 3, true);
			break;
		}
		default:
			SpriteComponent* sc = new SpriteComponent(this);
			sc->SetTexture(game->GetTexture("../Assets/Brock-2.png"));
			break;
	}

	mBoxComp = new BoxComponent(this);

	AABB myBox(Vector3(-32.0f, -32.0f, 0.f),
		Vector3(32.0f, 32.0f, 0.f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);

	GetGame()->SetBrock(this);
}

Brock::~Brock()
{
	GetGame()->RemoveBrock(this);
}

void Brock::UpdateActor(float deltaTime)
{
	mBoxComp->OnUpdateWorldTransform();

	if(mContainItem == ContainItem::EHatenaMushroom || mContainItem == ContainItem::EHatenaCoin)
	{
	

	}
	else
	{
		if (GetActionState() == ActionState::EDeath)
		{
			SetState(State::EDead);
		}
	}
	
}
