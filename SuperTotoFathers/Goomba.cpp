#include "Goomba.h"
#include "Game.h"
#include "Texture.h"
#include "AnimSpriteComponent.h"
#include "MoveComponent.h"


Goomba::Goomba(Game* game)
	:Actor(game)
	,moveSpeed(-100.0f,0.0f)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<class Texture*> anims = {
		game->GetTexture("../Assets/Goomba-1.png"),
		game->GetTexture("../Assets/Goomba-2.png"),
	};

	asc->SetAnimTextures(anims);
	asc->SetAnimRange("walk", 0, 1, true);
	asc->SetAnimFPS(5.0f);

	mc = new MoveComponent(this);
	mc->SetMass(1.0f);
	mc->SetVelocityLimit(Vector2(70.0f, 70.0f));
	mc->SetIsGravity(true);

}

void Goomba::UpdateActor(float deltaTime)
{
	mc->AddForce(moveSpeed);
}

void Goomba::ActorInput(const uint8_t* keyState)
{

}
