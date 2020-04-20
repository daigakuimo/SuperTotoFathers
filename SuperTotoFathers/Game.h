#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <glew.h>


struct enemyCollision
{
	enemyCollision();
	class CircleComponent* mCircle;
	class Actor* mOwner;
};

class Game
{
public:
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

	std::vector<enemyCollision> GetEnemys() { return mEnemys; }
	void SetEnemys(enemyCollision info) { mEnemys.emplace_back(info); }

	std::vector<class Brock*> GetBrocks() { return mBrocks; }
	void SetBrock(class Brock* brock) { mBrocks.emplace_back(brock); }
	void RemoveBrock(class Brock* brock);


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

	std::vector<enemyCollision> mEnemys;

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

	// Game-specific code
	class CameraActor* mCameraActor;
};


