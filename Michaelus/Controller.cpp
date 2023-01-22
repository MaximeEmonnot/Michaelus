#include "Controller.h"

Controller::Controller(std::weak_ptr<Pawn> pPawn)
	:
	pPawn(pPawn)
{
}

void Controller::Update()
{
}

std::shared_ptr<Pawn> Controller::GetPawn()
{
	return pPawn.lock();
}
