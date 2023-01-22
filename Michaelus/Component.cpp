#include "Component.h"

#include "Actor.h"

Component::Component(std::weak_ptr<Actor> pOwner)
	:
	pOwner(pOwner)
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

std::shared_ptr<Actor> Component::GetOwner() const
{
	return pOwner.lock();
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
	return GetOwner()->GetActorLocation() + transform.location;
}

FRotator Component::GetWorldRotation() const
{
	if (pParentComponent) return pParentComponent->GetRelativeRotation() + transform.rotation;
	return GetOwner()->GetActorRotation() + transform.rotation;
}

FTransform Component::GetTransform() const
{
	return transform;
}
