#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader, VertexArray* vertex)
{
	if (mTexture)
	{

		float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.0f, // top left
		 0.5f,  0.5f, 0.f, 0.5f, 0.0f, // top right
		 0.5f, -0.5f, 0.f, 0.5f, 0.5f, // bottom right
		-0.5f, -0.5f, 0.f, 0.f, 0.5f  // bottom left
		};

		vertex->ChangeVBO(vertices);
		vertex->SetActive();

		// Scale the quad by the width/height of texture
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();


		// Since all sprites use the same shader/vertices,
		// the game first sets them active before any sprite draws

		// Set world transform
		shader->SetMatrixUniform("uWorldTransform", world);

		mTexture->SetActive();


		// Draw quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	}
}


void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;

	// Set width/height
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}
