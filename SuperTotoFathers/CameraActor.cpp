#include "CameraActor.h"
#include "MoveComponent.h"
#include "SDL_scancode.h"
#include "Game.h"
#include "Shader.h"

CameraActor::CameraActor(Game* game)
	:Actor(game)
{
	mMoveComp = new MoveComponent(this);
	Matrix4 view = Matrix4::CreateTranslation(Vector3(-50.0f, 0, 0.0f));

	GetGame()->GetShader()->SetMatrixUniform("uCameraView", view);
}

void CameraActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// Compute new camera from this actor

	float nextX = -(mFollowActor->GetPosition().x);

	if (nextX < mBeforeX)
	{
		Matrix4 view = Matrix4::CreateTranslation(Vector3(nextX - 60.0f, 0, 0.0f));

		GetGame()->GetShader()->SetMatrixUniform("uCameraView", view);

		mBeforeX = nextX;
	}

}
