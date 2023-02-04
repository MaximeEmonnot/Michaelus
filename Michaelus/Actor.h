#pragma once
#include <memory>
#include <string>
#include <vector>

#include "CameraComponent.h"
#include "CameraManager.h"
#include "EngineException.h"
#include "Graphics.h"
#include "Transform.h"
#include "MeshComponent.h"

class Component;

class Actor
{
public:
	Actor() = default;
	Actor(const std::string& name);

	virtual void Update();

	template <class T>
	std::shared_ptr<T> CreateComponent()
	{
		std::shared_ptr<Component> newComponent = std::make_shared<T>(*this);
		components.emplace_back(newComponent);

		if (auto newCamera = std::dynamic_pointer_cast<CameraComponent>(newComponent))
			CAMERA.AddCamera(newCamera);
		if (auto newMesh = std::dynamic_pointer_cast<MeshComponent>(newComponent))
			GFX.AddMesh(newMesh);

		return std::dynamic_pointer_cast<T>(newComponent);
	}

	std::string GetName() const;
	std::shared_ptr<Component> GetRootComponent() const;

	template <class T>
	std::shared_ptr<T> GetComponentByClass() const
	{
		if (!std::is_base_of<Component, T>())
			throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for GetComponentByClass.");

		for (std::shared_ptr<Component> component : components)
			if (std::shared_ptr<T> comp = std::dynamic_pointer_cast<T>(component)) return comp;

		return nullptr;
	}
	template <class T>
	std::vector<std::shared_ptr<T>> GetComponentsByClass() const
	{
		if (!std::is_base_of<Component, T>())
			throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for GetComponentsByClass.");

		std::vector<std::shared_ptr<T>> out;

		for (std::shared_ptr<Component> component : components)
			if (std::shared_ptr<T> comp = std::dynamic_pointer_cast<T>(component)) out.emplace_back(comp);

		return out;
	}

	void SetActorLocation(const FVec3D& newLocation);
	void SetActorRotation(const FQuaternion& newRotation);
	void AddActorLocation(const FVec3D& offsetLocation);
	void AddActorRotation(const FQuaternion& offsetRotation);
	FVec3D GetActorLocation() const;
	FQuaternion GetActorRotation() const;
	FTransform GetActorTransform() const;

	FVec3D GetActorForwardVector() const;
	FVec3D GetActorRightVector() const;
	FVec3D GetActorUpVector() const;

protected:
	std::shared_ptr<Component> pRootComponent;
private:
	std::string name;
	std::vector<std::shared_ptr<Component>> components;
	FTransform transform;
};

