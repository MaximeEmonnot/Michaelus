#include "SceneComponent.h"

// Constructeur d�finissant l'acteur li� au Component
SceneComponent::SceneComponent(Actor& rOwner)
	:
	Component(rOwner)
{}

// M�thode Update() h�rit�e de Component : Simple m�thode concr�te sans calculs
void SceneComponent::Update()
{}
