#pragma once
#include "Component.h"
class Mesh;

/*
 * Classe MeshComponent
 * Component d�finissant un Mesh � afficher dans la sc�ne
 */
class MeshComponent :
    public Component
{
public:
    // Constructeur d�finissant l'acteur li� au Component
    MeshComponent(Actor& rOwner);

    // M�thode Update() h�rit�e de Component
    virtual void Update() override;

    // D�finition du Mesh
    void SetMesh(std::shared_ptr<Mesh> pNewMesh);
    // R�cup�ration du Mesh
    std::shared_ptr<Mesh> GetMesh() const;

private:
    std::shared_ptr<Mesh> pMesh;
};

