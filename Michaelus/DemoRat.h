#pragma once
#include "Actor.h"

class MeshComponent;

/*
 * Classe DemoRat
 * Démonstration d'un Actor possédant un Mesh de rat
 */
class DemoRat : public Actor
{
public:
	// Constructeur par défaut
	DemoRat() = default;
	// Constructeur définissant le nom de l'acteur
	DemoRat(const std::string& name);

	// Méthode Update() de l'acteur, surchargée
	virtual void Update() override;

private:
	std::shared_ptr<MeshComponent> pMeshComponent;
};

