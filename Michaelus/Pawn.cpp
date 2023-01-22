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

template<class T>
void Pawn::SetController()
{
	if (!std::is_base_of<Controller, T>())
		throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Controller Class. Please check your call for SetController.");

	pController = std::make_shared<T>(std::make_shared<Pawn>(*this));
}


std::shared_ptr<Controller> Pawn::GetController() const
{
	return pController;
}
