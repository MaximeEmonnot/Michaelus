#pragma once
#include "Actor.h"

/*
 * Classe DemoShrek
 * Démonstration d'un Actor possédant un Mesh de Shrek
 */
class DemoShrek :
    public Actor
{
public:
    // Constructeur par défaut
    DemoShrek() = default;
    // Constructeur définissant le nom de l'acteur
    DemoShrek(const std::string & name);

    // Méthode Update() de l'acteur, surchargée
    virtual void Update() override;

private:
    std::shared_ptr<MeshComponent> pMeshComponent;
};

