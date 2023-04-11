#include "Pawn.h"
#include "Controller.h"

#include "EngineException.h"

// Constructeur définissant le nom du pion
Pawn::Pawn(const std::string& name)
	:
	Actor(name)
{}

// Méthode Update() du pion, surchargée : On appelle Update() de la classe parent (equivalent super.Update() en Java), puis le Update() du Controller
void Pawn::Update()
{
	Actor::Update();

	if (pController) pController->Update();
}

// Récupération du Controller
std::shared_ptr<Controller> Pawn::GetController() const
{
	return pController;
}
