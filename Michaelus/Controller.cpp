#include "Controller.h"

#include "Pawn.h"

Controller::Controller(Pawn& pawn)
	:
	pawn(pawn)
{
}

void Controller::Update()
{
}

Pawn& Controller::GetPawn() const
{
	return pawn;
}

