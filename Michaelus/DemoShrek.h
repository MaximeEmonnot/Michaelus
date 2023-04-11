#pragma once
#include "Actor.h"

/*
 * Classe DemoShrek
 * D�monstration d'un Actor poss�dant un Mesh de Shrek
 */
class DemoShrek :
    public Actor
{
public:
    // Constructeur par d�faut
    DemoShrek() = default;
    // Constructeur d�finissant le nom de l'acteur
    DemoShrek(const std::string & name);

    // M�thode Update() de l'acteur, surcharg�e
    virtual void Update() override;

private:
    std::shared_ptr<MeshComponent> pMeshComponent;
};

