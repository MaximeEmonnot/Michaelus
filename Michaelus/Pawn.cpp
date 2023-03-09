#include "Pawn.h"
#include "Controller.h"

#include "EngineException.h"

Pawn::Pawn(const std::string& name)
	:
	Actor(name)
{
}

void Pawn::Update()
{
	Actor::Update();

	if (pController) pController->Update();
}

std::shared_ptr<Controller> Pawn::GetController() const
{
	return pController;
}
