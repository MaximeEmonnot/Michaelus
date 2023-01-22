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

std::shared_ptr<Actor> Component::GetOwner() const
{
	return pOwner.lock();
}
