#pragma once
#include <vector>
#include "Math.h"

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	enum ActionState
	{
		EStop,
		EDeath,
		EWalk,
		EJump,
		EFall,
		EGoal,
	};

	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);

	// Updates all the components attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);

	// Any actor-specific update code 
	virtual void UpdateActor(float deltaTime);

	// ProcessInput function called from Game (not override)
	void ProcessInput(const uint8_t* keyState);

	// Any actor-specific input code (override)
	virtual void ActorInput(const uint8_t* keyState);

	// Getter/setter
	const Vector2& GetPosition()     const { return mPosition; }
	void  SetPosition(const Vector2& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void  SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	float GetRotation()              const { return mRotation; }
	void  SetRotation(float rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

	int   GetCurrentAction()         const { return mCurrentAction; }
	void  SetCurrentAction(int currAction) { mCurrentAction = currAction; }

	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), Math::Sin(mRotation)); }

	State GetState() const { return mState; }
	void  SetState(State state) { mState = state; }

	ActionState GetActionState() const { return mActionState; }
	void SetActionState(ActionState state) { mActionState = state; }

	class Game* GetGame() { return mGame; }

	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// Actor's state
	State mState;

	// Actor's action state
	ActionState mActionState;

	// Transform
	Matrix4 mWorldTransform;
	Vector2 mPosition;
	float mScale;
	float mRotation;
	bool  mRecomputeWorldTransform;

	// Action
	int mCurrentAction;

	std::vector<class Component*> mComponents;
	class Game* mGame;

	// 当たり判定のコンポーネントを入れる
	std::vector<class BoxComponent*> mBoxCompList;
	std::vector<class CircleComponent*> mCircleCompList;

};