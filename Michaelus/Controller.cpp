#include "Controller.h"

#include "Pawn.h"

// Constructeur définissant le Pawn lié au Controller
Controller::Controller(Pawn& pawn)
	:
	pawn(pawn)
{}

// Récupération du Pawn possédé
Pawn& Controller::GetPawn() const
{
	return pawn;
}

