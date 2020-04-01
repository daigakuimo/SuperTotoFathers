#include "Player.h"
#include "Game.h"
#include "Texture.h"
#include "InputComponent.h"
#include "AnimSpriteComponent.h"
#include "CameraActor.h"


Player::Player(Game* game)
	:Actor(game)
{
	// Create a sprite component
	//SpriteComponent* sc = new SpriteComponent(this, 150);
	//sc->SetTexture(game->GetTexture("../Assets/TOTO1-1.png"));

	// Create an animation sprite compoent
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<class Texture*> anims = {
		game->GetTexture("../Assets/Toto1-1.png"),
		game->GetTexture("../Assets/Toto1-2.png"),
		game->GetTexture("../Assets/Toto1-3.png"),
	};

	asc->SetAnimTextures(anims);
	asc->SetAnimRange("stop", 0, 0, true);
	asc->SetAnimRange("walk", 1, 2, true);

	// Create an input component and set keys/speed
	ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_D);
	ic->SetBackKey(SDL_SCANCODE_A);
	ic->SetJumpKey(SDL_SCANCODE_SPACE);
	ic->SetMaxJumpHeight(124.0f);
	ic->SetMass(1.0f);
	ic->SetVelocityLimit(Vector2(400.0f, 400.0f));
	ic->SetIsGravity(true);

}

void Player::UpdateActor(float deltaTime)
{

	if (Math::NearZero(ic->GetForwardSpeed().x))
	{
		SetCurrentAction(mACTION_STOP);
	}
	else
	{
		SetCurrentAction(mACTION_WALK);
	}

	// ‰æ–Ê‚Ì¶’[‚Å‚ÍŽ~‚Ü‚é
	float cameraPos = GetGame()->GetCameraActor()->GetBeforeX();
	Vector2 currentPos = GetPosition();
	if (currentPos.x < -cameraPos - 445.0f) 
	{ 
		SetPosition(Vector2(-cameraPos - 445.0f, currentPos.y));
	}
}

void Player::ActorInput(const uint8_t* keyState)
{

}