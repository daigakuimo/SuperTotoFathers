#pragma once

#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

class BGSpriteComponent : public SpriteComponent
{
public:
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);

	// Update/draw override from parent
	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;

	// Set the textures used for the background
	void SetBGTextures(const std::vector<class Texture*>& textures);

	// Get/set screen size and scroll speed
	void  SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void  SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }

private:
	// Struct to encpsulate each bg image and its offset
	struct BGTexture
	{
		class Texture* mTexture;
		Vector2 mOffset;
	};

	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;

};