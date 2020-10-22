#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <glew.h>
#include "SoundEvent.h"


class Game
{
public:

	enum Scene
	{
		ETitle,
		EMain,
		EGameOver,
		EEnd
	};

	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	class Texture* GetTexture(const std::string& fileName);
	class Shader* GetShader() { return mSpriteShader; }
	class CameraActor* GetCameraActor() { return mCameraActor; }
	class PhysWorld* GetPhysWorld() { return mPhysWorld; }

	std::vector<class BoxComponent*> GetStageBoxes() { return mStageBoxes; }
	void SetStageBoxes(class BoxComponent* box) { mStageBoxes.emplace_back(box); }

	std::vector<class CircleComponent*> GetEnemys() { return mEnemys; }
	void SetEnemys(class CircleComponent* circle) { mEnemys.emplace_back(circle); }
	void RemoveEnemy(class CircleComponent* circle);

	std::vector<class Brock*> GetBrocks() { return mBrocks; }
	void SetBrock(class Brock* brock) { mBrocks.emplace_back(brock); }
	void RemoveBrock(class Brock* brock);

	std::vector<class Item*> GetItems() { return mItems; }
	void SetItem(class Item* item) { mItems.emplace_back(item); }
	void RemoveItem(class Item* item);

	class Goal* GetGoal() { return mGoal; }
	void SetGoal(class Goal* goal) { mGoal = goal; }

	void ChangeSceneToMain() { mScene = Scene::EMain; }
	void ChangeSceneToEnd() { mScene = Scene::EEnd; }
	void ChangeSceneToTitl() { mScene = Scene::ETitle; }

	class AudioSystem* GetAudioSystem() { return mAudioSystem; }

	void stopBGM() { mMusicEvent.SetPaused(true);}


private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool LoadShaders();
	void CreateSpriteVerts();
	void LoadData();
	void UnloadData();

	std::vector<class Actor*> mActors;

	std::vector<class Actor*> mPendingActors;

	std::vector<class SpriteComponent*> mSprites;

	std::vector<class BoxComponent*> mStageBoxes;

	std::vector<class Brock*> mBrocks;

	std::vector<class CircleComponent*> mEnemys;

	std::vector<class Item*> mItems;

	std::unordered_map<std::string, class Texture*> mTextures;

	class Shader* mSpriteShader;
	class VertexArray* mSpriteVerts;

	SDL_Window* mWindow;
	SDL_GLContext mContext;
	Uint32        mTicksCount;

	bool mIsRunning;
	bool mUpdatingActors;

	class Player* mPlayer;

	class Goomba* mGoomba;

	class PhysWorld* mPhysWorld;

	class StageManager* mStage;

	class Goal* mGoal;

	// Game-specific code
	class CameraActor* mCameraActor;

	Scene mScene;

	class AudioSystem* mAudioSystem;
	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;
};


