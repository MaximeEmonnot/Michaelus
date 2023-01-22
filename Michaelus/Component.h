#pragma once
#include <memory>

#include "Transform.h"
class Actor;

class Component
{
public:
	Component(Actor& rOwner);

	virtual void Update() = 0;

	void AttachTo(std::shared_ptr<Component> pNewParentComponent);
	void Detach();

	Actor& GetOwner();

	void AddRelativeLocation(const FVec3D& offsetLocation);
	void AddRelativeRotation(const FRotator& offsetRotation);

	FVec3D GetRelativeLocation() const;
	FRotator GetRelativeRotation() const;
	FVec3D GetWorldLocation() const;
	FRotator GetWorldRotation() const;

	FTransform GetTransform() const;

private:
	Actor& rOwner;

	std::shared_ptr<Component> pParentComponent;

	FTransform transform;
};
