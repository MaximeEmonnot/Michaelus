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

template<class T, typename ...Args>
std::shared_ptr<T> Actor::CreateComponent(Args... args)
{
	if (!std::is_base_of<Component, T>())
		throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for CreateComponent.");

	std::shared_ptr<T> newComponent = std::make_unique<T>(args);
	components.emplace(newComponent);

	return newComponent;
}

template <class T>
std::shared_ptr<T> Actor::GetComponentByClass() const
{
	if (!std::is_base_of<Component, T>())
		throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for GetComponentByClass.");

	for (std::shared_ptr<Component> component : components)
		if (std::shared_ptr<T> comp = std::dynamic_pointer_cast<T>(component)) return comp;

	return nullptr;
}

template<class T>
std::vector<std::shared_ptr<T>> Actor::GetComponentsByClass() const
{
	if (!std::is_base_of<Component, T>())
		throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for GetComponentsByClass.");

	std::vector<std::shared_ptr<T>> out;

	for (std::shared_ptr<Component> component : components)
		if (std::shared_ptr<T> comp = std::dynamic_pointer_cast<T>(component)) out.emplace_back(comp);

	return out;
}
