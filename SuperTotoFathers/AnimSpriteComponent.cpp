#include "AnimSpriteComponent.h"
#include "Actor.h"
#include "Player.h"
#include "Math.h"
#include "Texture.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(10.0f)
{
}

void AnimSpriteComponent::SetAnimRange(const std::string& action, int minRange, int maxRange, bool isRoop)
{
	AnimRange temp;

	temp.actionName = action;
	temp.minRange = minRange;
	temp.maxRange = maxRange;
	temp.isRoop = isRoop;

	mAnimRanges.emplace_back(temp);
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimRanges.size() > 0)
	{
		if (mAnimTextures.size() > 0)
		{
			int currentAction = mOwner->GetCurrentAction();

			if (currentAction != -1)
			{
				// Update the current frame based on frame rate and delta time
				mCurrFrame += mAnimFPS * deltaTime;

				while (mCurrFrame >= (mAnimRanges[currentAction].maxRange - mAnimRanges[currentAction].minRange + 1))
				{
					mCurrFrame -= (mAnimRanges[currentAction].maxRange - mAnimRanges[currentAction].minRange + 1);
				}

				SetTexture(mAnimTextures[static_cast<int>(mCurrFrame + mAnimRanges[currentAction].minRange)]);
			}
		}
	}
	else
	{
		if (mAnimTextures.size() > 0)
		{
			// Update the current frame based on frame rate and delta time
			mCurrFrame += mAnimFPS * deltaTime;

			// Wrap current frame if needed
			while (mCurrFrame >= mAnimTextures.size())
			{
				mCurrFrame -= mAnimTextures.size();
			}

			SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
		}
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<class Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}