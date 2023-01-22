#include "Component.h"

#include "Actor.h"

Component::Component(Actor& rOwner)
	:
	rOwner(rOwner)
{
}

void Component::AttachTo(std::shared_ptr<Component> pNewParentComponent)
{
	pParentComponent = pNewParentComponent;
}

void Component::Detach()
{
	pParentComponent = nullptr;
}

Actor& Component::GetOwner()
{
	return rOwner;
}

void Component::AddRelativeLocation(const FVec3D& offsetLocation)
{
	transform.location += offsetLocation;
}

void Component::AddRelativeRotation(const FRotator& offsetRotation)
{
	transform.rotation += offsetRotation;
}

FVec3D Component::GetRelativeLocation() const
{
	return transform.location;
}

FRotator Component::GetRelativeRotation() const
{
	return transform.rotation;
}

FVec3D Component::GetWorldLocation() const
{
	if (pParentComponent) return pParentComponent->GetRelativeLocation() + transform.location;
	return rOwner.GetActorLocation() + transform.location;
}

FRotator Component::GetWorldRotation() const
{
	if (pParentComponent) return pParentComponent->GetRelativeRotation() + transform.rotation;
	return rOwner.GetActorRotation() + transform.rotation;
}

FTransform Component::GetTransform() const
{
	return transform;
}
