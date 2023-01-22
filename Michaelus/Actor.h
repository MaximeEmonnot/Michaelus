#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Transform.h"

class Component;

class Actor
{
public:
	Actor() = default;
	Actor(const std::string& name);

	virtual void Update();

	template <class T, typename ...Args>
	std::shared_ptr<T> CreateComponent(Args... args);

	std::string GetName() const;
	std::shared_ptr<Component> GetRootComponent() const;

	template <class T>
	std::shared_ptr<T> GetComponentByClass() const;
	template <class T>
	std::vector<std::shared_ptr<T>> GetComponentsByClass() const;

	void SetActorLocation(const FVec3D& newLocation);
	void SetActorRotation(const FRotator& newRotation);
	void AddActorLocation(const FVec3D& offsetLocation);
	void AddActorRotation(const FRotator& offsetRotation);
	FVec3D GetActorLocation() const;
	FRotator GetActorRotation() const;

protected:
	std::shared_ptr<Component> pRootComponent;
private:
	std::string name;
	std::vector<std::shared_ptr<Component>> components;
	FTransform transform;
};

