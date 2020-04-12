#pragma once
#include "SpriteComponent.h"
#include <unordered_map>
#include <vector>
#include "Actor.h"

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);

	void Update(float deltaTime) override;

	void SetAnimTextures(const std::vector<class Texture*>& textures);

	float GetAnimFPS() const { return mAnimFPS; }
	void  SetAnimFPS(float fps) { mAnimFPS = fps; }
	void  SetAnimRange(Actor::ActionState state, int minRange, int maxRange, bool isRoop);

private:

	// Animation range
	struct AnimRange
	{
		Actor::ActionState state;
		int         minRange;
		int         maxRange;
		bool        isRoop;
	};

	// All textures in the animation
	std::vector<class Texture*> mAnimTextures;

	// Animation ranges
	std::vector<AnimRange> mAnimRanges;

	// Current frame displayed
	float mCurrFrame;

	// Animation frame rate
	float mAnimFPS;


};