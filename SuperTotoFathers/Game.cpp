#include "Game.h"
#include <SDL_image.h>
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Random.h"
#include "Texture.h"
#include "Player.h"
#include "Goomba.h"
#include "TileMapComponent.h"
#include "CameraActor.h"
#include "PhysWorld.h"
#include "BoxComponent.h"
#include "CircleComponent.h"
#include "StageManager.h"
#include "Item.h"
#include "AudioSystem.h"
#include "AudioComponent.h"

Game::Game()
	:mWindow(nullptr)
	, mSpriteShader(nullptr)
	, mPhysWorld(nullptr)
	, mPlayer(nullptr)
	, mGoomba(nullptr)
	, mCameraActor(nullptr)
	, mStage(nullptr)
	, mSpriteVerts(nullptr)
	, mContext()
	, mIsRunning(true)
	, mUpdatingActors(false)
	, mTicksCount(0)
	, mAudioSystem(nullptr)
{

}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Super Toto Father", 100, 100,
		1024, 768, SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);

	// Create the physics world
	mPhysWorld = new PhysWorld(this);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

	// Make sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// Create quad for drawing sprites
	CreateSpriteVerts();

	mScene = Scene::ETitle;

	// Create the audio system
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();


	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	if (mScene == Scene::ETitle)
	{
		if (keyState[SDL_SCANCODE_SPACE])
		{
			// UnloadData();
			ChangeSceneToMain();
			LoadData();
		}
	}
	
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// Update all actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// Move any pending actors to mActors
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();


	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}

	// Update audio system
	mAudioSystem->Update(deltaTime);
}


void Game::GenerateOutput()
{
	// Set the clear color to grey
	glClearColor(0.0f, 0.8f, 0.86f, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw all sprite components
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set shader/vao as active

	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader, mSpriteVerts);
	}

	// Swap the buffers
	SDL_GL_SwapWindow(mWindow);
}

bool Game::LoadShaders()
{
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("../SuperTotoFathers/Shaders/Sprite.vert", "../SuperTotoFathers/Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();
	// Set the view-projection matrix
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);
	return true;
}

void Game::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.0f, // top left
		 0.5f,  0.5f, 0.f, 1.f, 0.0f, // top right
		 0.5f, -0.5f, 0.f, 1.f, 1.f,  // bottom right
		-0.5f, -0.5f, 0.f, 0.f, 1.f   // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData()
{
	if (mScene == Scene::ETitle)
	{
		// Start music
		// mMusicEvent = mAudioSystem->PlayEvent("event:/Music");
	}
	else if (mScene == Scene::EMain)
	{
		mPlayer = new Player(this);
		mPlayer->SetPosition(Vector2(0.0f, -243.0f));
		mPlayer->SetScale(1.0f);

		mCameraActor = new CameraActor(this);
		mCameraActor->SetActor(mPlayer);

		mStage = new StageManager(this);
		mStage->SetPosition(Vector2(0.0f, 0.0f));
		mStage->SetPlayer(mPlayer);
		// Create the tile map
		TileMapComponent* tm = new TileMapComponent(mStage);
		class Texture* tiletex = GetTexture("../Assets/TileMap1.png");
		tm->SetTileMap(tiletex);
		tm->SetChasePlayer(mPlayer);

		// Start music
		mMusicEvent = mAudioSystem->PlayEvent("event:/canon");
	}
	else if (mScene == Scene::EEnd)
	{
		// Start music
		//mMusicEvent = mAudioSystem->PlayEvent("event:/Music");
	}
	
}


void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}


	mTextures.clear();
}

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

void Game::Shutdown()
{
	UnloadData();
	delete mSpriteVerts;
	mSpriteShader->UnLoad();
	delete mSpriteShader;
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// If we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent * sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent * sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Game::RemoveEnemy(class CircleComponent* circle)
{
	auto iter = std::find(mEnemys.begin(), mEnemys.end(), circle);
	if (iter != mEnemys.end())
	{
		mEnemys.erase(iter);
	}
}

void Game::RemoveBrock(class Brock* brock)
{
	auto iter = std::find(mBrocks.begin(), mBrocks.end(), brock);
	if (iter != mBrocks.end())
	{
		mBrocks.erase(iter);
	}
}

void Game::RemoveItem(class Item* item)
{
	auto iter = std::find(mItems.begin(), mItems.end(), item);
	if (iter != mItems.end())
	{
		mItems.erase(iter);
	}
}