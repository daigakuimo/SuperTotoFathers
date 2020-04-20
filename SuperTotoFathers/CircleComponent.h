#pragma once
#include "Component.h"
#include "Collision.h"

class CircleComponent : public Component
{
public:
	CircleComponent(class Actor* owner, int updateOrder = 100);
	~CircleComponent();

	void OnUpdateWorldTransform() override;

	void SetObjectCircle(const Sphere& model) { mObjectCircle = model; }
	const Sphere& GetWorldCircle() const { return mWorldCircle; }

private:
	Sphere mObjectCircle;
	Sphere mWorldCircle;
};