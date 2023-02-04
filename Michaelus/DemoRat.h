#pragma once
#include "Actor.h"

class MeshComponent;

class DemoRat : public Actor
{
public:
	DemoRat() = default;
	DemoRat(const std::string& name);

	virtual void Update() override;

private:
	std::shared_ptr<MeshComponent> pMeshComponent;

	float angle = 0.f;
};

