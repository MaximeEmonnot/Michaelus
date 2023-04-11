#include "Pawn.h"
#include "Controller.h"

#include "EngineException.h"

// Constructeur d�finissant le nom du pion
Pawn::Pawn(const std::string& name)
	:
	Actor(name)
{}

// M�thode Update() du pion, surcharg�e : On appelle Update() de la classe parent (equivalent super.Update() en Java), puis le Update() du Controller
void Pawn::Update()
{
	Actor::Update();

	if (pController) pController->Update();
}

// R�cup�ration du Controller
std::shared_ptr<Controller> Pawn::GetController() const
{
	return pController;
}
