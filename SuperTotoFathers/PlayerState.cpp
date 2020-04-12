// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PlayerState.h"
#include "PlayerStateComponent.h"
#include <SDL_log.h>

void PlayerOnGround::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
	bool dead = true;
	if (dead)
	{
		mOwner->ChangeState("Jump");
	}
}

void PlayerOnGround::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void PlayerOnGround::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void PlayerJump::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
}

void PlayerJump::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void PlayerJump::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void PlayerFall::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
}

void PlayerFall::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void PlayerFall::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}
