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

void Component::AddRelativeRotation(const FQuaternion& offsetRotation)
{
	transform.rotation = transform.rotation * offsetRotation.GetUnit();
}

FVec3D Component::GetRelativeLocation() const
{
	return transform.location;
}

FQuaternion Component::GetRelativeRotation() const
{
	return transform.rotation;
}

FVec3D Component::GetWorldLocation() const
{
	if (pParentComponent) 
		return pParentComponent->GetWorldLocation() + transform.location;
	return rOwner.GetActorLocation() + transform.location;
}

FQuaternion Component::GetWorldRotation() const
{
	if (pParentComponent) return transform.rotation.GetUnit() * pParentComponent->GetWorldRotation().GetUnit();
	return transform.rotation.GetUnit() * rOwner.GetActorRotation().GetUnit();
}

FTransform Component::GetTransform() const
{
	return transform;
}

FVec3D Component::GetForwardVector() const
{
	return GetWorldRotation().RotateVector(FVec3D(1.f, 0.f ,0.f));
}

FVec3D Component::GetRightVector() const
{
	return GetWorldRotation().RotateVector(FVec3D(0.f, 1.f, 0.f));
}

FVec3D Component::GetUpVector() const
{
	return GetWorldRotation().RotateVector(FVec3D(0.f, 0.f, 1.f));
}
