#pragma once

class PlayerState
{
public:
	PlayerState(class PlayerStateComponent* owner)
		:mOwner(owner)
	{
	}

	// State-specific behavior
	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	// Getter for string name of state
	virtual const char* GetName() const = 0;

protected:
	class PlayerStateComponent* mOwner;
};

class PlayerOnGround : public PlayerState
{
public:
	PlayerOnGround(class PlayerStateComponent* owner)
		:PlayerState(owner)
	{
	}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return "OnGround";
	}
};

class PlayerJump : public PlayerState
{
public:
	PlayerJump(class PlayerStateComponent* owner)
		:PlayerState(owner)
	{
	}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return "Jump";
	}
};

class PlayerFall : public PlayerState
{
public:
	PlayerFall(class PlayerStateComponent* owner)
		:PlayerState(owner)
	{
	}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return "Fall";
	}
};