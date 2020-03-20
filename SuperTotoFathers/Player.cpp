#include "Player.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Texture.h"
#include "InputComponent.h"


Player::Player(Game* game)
	:Actor(game)
{
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("../Assets/TOTO1-1.png"));

	// Create an input component and set keys/speed
	InputComponent* ic = new InputComponent(this, 1.0f, Vector2(400.0f,400.0f));
	ic->SetForwardKey(SDL_SCANCODE_D);
	ic->SetBackKey(SDL_SCANCODE_A);
	ic->SetMaxForwardSpeed(500.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);
}

void Player::UpdateActor(float deltaTime)
{
}

void Player::ActorInput(const uint8_t* keyState)
{

}