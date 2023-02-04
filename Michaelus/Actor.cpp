#include "Actor.h"

#include "CameraComponent.h"
#include "CameraManager.h"
#include "Component.h"
#include "EngineException.h"
#include "Graphics.h"
#include "MeshComponent.h"

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

void Actor::SetActorRotation(const FQuaternion& newRotation)
{
	transform.rotation = newRotation;
}

void Actor::AddActorLocation(const FVec3D& offsetLocation)
{
	transform.location += offsetLocation;
}

void Actor::AddActorRotation(const FQuaternion& offsetRotation)
{
	transform.rotation = transform.rotation * offsetRotation;
}

FVec3D Actor::GetActorLocation() const
{
	return transform.location;
}

FQuaternion Actor::GetActorRotation() const
{
	return transform.rotation;
}

FTransform Actor::GetActorTransform() const
{
	return transform;
}

FVec3D Actor::GetActorForwardVector() const
{
	return transform.rotation.UnrotateVector(FVec3D(1.f, 0.f, 0.f));
}

FVec3D Actor::GetActorRightVector() const
{
	return transform.rotation.UnrotateVector(FVec3D(0.f, 1.f, 0.f));
}

FVec3D Actor::GetActorUpVector() const
{
	return transform.rotation.UnrotateVector(FVec3D(0.f, 0.f, 1.f));
}
