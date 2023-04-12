#pragma once

// Forward declarations
class Pawn;

/*
 * Classe Controller
 * Permet la possession/contrôle d'un pion
 * Définit une stratégie de comportement pour un Pawn
 */
class Controller
{
public:
	// Constructeur par défaut
	Controller() = default;
	// Constructeur définissant le Pawn lié au Controller
	Controller(Pawn& pawn);

	// Méthode virtuelle pure (=abstraite) Update() du Controller
	virtual void Update() = 0;

protected:
	// Récupération du Pawn possédé
	Pawn& GetPawn() const;

private:
	Pawn& pawn;
};

