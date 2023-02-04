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
	void AddRelativeRotation(const FQuaternion& offsetRotation);

	FVec3D GetRelativeLocation() const;
	FQuaternion GetRelativeRotation() const;
	FVec3D GetWorldLocation() const;
	FQuaternion GetWorldRotation() const;

	FTransform GetTransform() const;

	FVec3D GetForwardVector() const;
	FVec3D GetRightVector() const;
	FVec3D GetUpVector() const;

private:
	Actor& rOwner;

	std::shared_ptr<Component> pParentComponent;

	FTransform transform;
};
