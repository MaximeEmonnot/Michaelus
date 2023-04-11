#pragma once
#include "Actor.h"

class MeshComponent;

/*
 * Classe DemoRat
 * D�monstration d'un Actor poss�dant un Mesh de rat
 */
class DemoRat : public Actor
{
public:
	// Constructeur par d�faut
	DemoRat() = default;
	// Constructeur d�finissant le nom de l'acteur
	DemoRat(const std::string& name);

	// M�thode Update() de l'acteur, surcharg�e
	virtual void Update() override;

private:
	std::shared_ptr<MeshComponent> pMeshComponent;
};

