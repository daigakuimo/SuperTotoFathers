#include "Brock.h"
#include "SpriteComponent.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include "Texture.h"
#include "BoxComponent.h"
#include "CameraActor.h"
#include "Mushroom.h"
#include "Coin.h"
#include <SDL_Log.h>

Brock::Brock(class Game* game, int item)
	: Actor(game)
	, mIsPutOutItem(false)
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

	if (GetActionState() == ActionState::EDeath && !mIsPutOutItem)
	{
		if (mContainItem == ContainItem::EHatenaMushroom)
		{
			mIsPutOutItem = true;
			class Mushroom* mushroom = new Mushroom(GetGame());
			mushroom->SetPosition(Vector2(GetPosition().x, GetPosition().y + 64.0f));
			mushroom->SetScale(1.0f);
		}
		else if (mContainItem == ContainItem::EHatenaCoin)
		{
			mIsPutOutItem = true;
			class Coin* coin = new Coin(GetGame());
			coin->SetPosition(Vector2(GetPosition().x, GetPosition().y + 64.0f));
			coin->SetScale(1.0f);
		}
		else
		{
			SetState(State::EDead);
			mIsPutOutItem = true;
		}
	}

	// 画面の画面外に出たらインスタンスを削除
	float cameraPos = GetGame()->GetCameraActor()->GetBeforeX();
	Vector2 currentPos = GetPosition();
	if (currentPos.x < -cameraPos - 573.0f)
	{
		SetState(State::EDead);
	}
}
