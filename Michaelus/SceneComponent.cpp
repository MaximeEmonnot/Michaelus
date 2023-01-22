#include "SceneComponent.h"

SceneComponent::SceneComponent(std::weak_ptr<Actor> pOwner)
	:
	Component(pOwner)
{
}

void SceneComponent::Update()
{
}
