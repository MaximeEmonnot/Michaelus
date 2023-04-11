#include "SceneComponent.h"

// Constructeur définissant l'acteur lié au Component
SceneComponent::SceneComponent(Actor& rOwner)
	:
	Component(rOwner)
{}

// Méthode Update() héritée de Component : Simple méthode concrète sans calculs
void SceneComponent::Update()
{}
