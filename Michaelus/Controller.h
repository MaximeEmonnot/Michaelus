#pragma once

// Forward declarations
class Pawn;

/*
 * Classe Controller
 * Permet la possession/contr�le d'un pion
 * D�finit une strat�gie de comportement pour un Pawn
 */
class Controller
{
public:
	// Constructeur par d�faut
	Controller() = default;
	// Constructeur d�finissant le Pawn li� au Controller
	Controller(Pawn& pawn);

	// M�thode virtuelle pure (=abstraite) Update() du Controller
	virtual void Update() = 0;

protected:
	// R�cup�ration du Pawn poss�d�
	Pawn& GetPawn() const;

private:
	Pawn& pawn;
};

