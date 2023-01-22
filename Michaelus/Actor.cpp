#include "Actor.h"

#include "Component.h"
#include "EngineException.h"

Actor::Actor(const std::string& name)
	:
	name(name)
{
}

void Actor::Update()
{
	for (std::shared_ptr<Component> component : components) component->Update();
}

std::string Actor::GetName() const
{
	return name;
}

std::shared_ptr<Component> Actor::GetRootComponent() const
{
	return pRootComponent;
}

void Actor::SetActorLocation(const FVec3D& newLocation)
{
	transform.location = newLocation;
}

void Actor::SetActorRotation(const FRotator& newRotation)
{
	transform.rotation = newRotation;
}

void Actor::AddActorLocation(const FVec3D& offsetLocation)
{
	transform.location += offsetLocation;
}

void Actor::AddActorRotation(const FRotator& offsetRotation)
{
	transform.rotation += offsetRotation;
}

FVec3D Actor::GetActorLocation() const
{
	return transform.location;
}

FRotator Actor::GetActorRotation() const
{
	return transform.rotation;
}

FTransform Actor::GetActorTransform() const
{
	return transform;
}
