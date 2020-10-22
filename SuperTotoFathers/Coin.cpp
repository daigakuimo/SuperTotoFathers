#include "Coin.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Texture.h"
#include "BoxComponent.h"
#include "CameraActor.h"
#include "MoveComponent.h"
#include "PhysWorld.h"
#include <SDL_Log.h>

Coin::Coin(class Game* game)
	:Item(game)
	, moveSpeed(0.0f, 100.0f)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Coin-1.png"));

	AABB myBox(Vector3(-16.0f, -32.0f, 0.f),
		Vector3(16.0f, 32.0f, 0.f));
	SetBoxComp(myBox);

	mc = new MoveComponent(this);
	mc->SetMass(1.0f);
	mc->SetVelocityLimit(Vector2(70.0f, 70.0f));

	GetGame()->SetItem(this);
}

void Coin::UpdateActor(const float deltatime)
{
	GetBoxComp()->OnUpdateWorldTransform();

	// 画面の画面外に出たらインスタンスを削除
	float cameraPos = GetGame()->GetCameraActor()->GetBeforeX();
	Vector2 currentPos = GetPosition();
	if (currentPos.x < -cameraPos - 573.0f)
	{
		SetState(State::EDead);
	}
}

void Coin::CollisionPlayer(class Actor* actor)
{
	//actor->SetScale(2.0f);
	SetState(State::EDead);
}




