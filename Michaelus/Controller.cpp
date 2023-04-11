#include "Controller.h"

#include "Pawn.h"

// Constructeur d�finissant le Pawn li� au Controller
Controller::Controller(Pawn& pawn)
	:
	pawn(pawn)
{}

// R�cup�ration du Pawn poss�d�
Pawn& Controller::GetPawn() const
{
	return pawn;
}

