#pragma once
#include "Actor.h"

class MeshComponent;

class TestActor : public Actor
{
public:
	TestActor() = default;
	TestActor(const std::string& name);

	virtual void Update() override;

private:
	std::shared_ptr<MeshComponent> pMeshComponent;

	float angle = 0.f;
};

