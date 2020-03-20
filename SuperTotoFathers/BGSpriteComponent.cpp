#include "BGSpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Shader.h"
#include "Texture.h"

BGSpriteComponent::BGSpriteComponent(class Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mScrollSpeed(0.0f)
{

}

void BGSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	for (auto& bg : mBGTextures)
	{
		// Update the x offset
		bg.mOffset.x += mScrollSpeed * deltaTime;

		if (bg.mOffset.x < -mScreenSize.x)
		{
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
		
	}
}

void BGSpriteComponent::Draw(class Shader* shader)
{
	// Scale the quad by the width/height of texture
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(mScreenSize.x),
		static_cast<float>(mScreenSize.y),
		1.0f
	);

	for (auto& bg : mBGTextures)
	{
	    float x = mOwner->GetPosition().x - mScreenSize.x / 2 + bg.mOffset.x;
		float y = mOwner->GetPosition().y - mScreenSize.y / 2 + bg.mOffset.y;
		Matrix4 translationMat = Matrix4::CreateTranslation(Vector3(x, y, 0.0f));
		
		Matrix4 world = scaleMat * translationMat;;

		shader->SetMatrixUniform("uWorldTransform", world);

		bg.mTexture->SetActive();

		// Draw quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}


void BGSpriteComponent::SetBGTextures(const std::vector<class Texture*>& textures)
{
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.mTexture = tex;

		// Each texture is screen width in offset
		temp.mOffset.x = count * mScreenSize.x;
		temp.mOffset.y = 0;
		mBGTextures.emplace_back(temp);
		count++;
	}
}