#pragma once
#include <memory>
class Actor;

class Component
{
public:
	Component(std::weak_ptr<Actor> pOwner);

	virtual void Update() = 0;

	void AttachTo(std::shared_ptr<Component> pNewParentComponent);

	std::shared_ptr<Actor> GetOwner() const;

private:
	std::weak_ptr<Actor> pOwner;

	std::shared_ptr<Component> pParentComponent;
};
