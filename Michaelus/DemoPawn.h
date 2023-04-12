#pragma once
#include "Pawn.h"

// Forward declarations
class SceneComponent;
class CameraComponent;

/*
 * Classe DemoPawn
 * D�monstration d'un pion pr�sent dans la sc�ne, poss�dant une cam�ra pour l'affichage
 */
class DemoPawn :
    public Pawn
{
public:
    // Constructeur d�finissant le nom du pion
    DemoPawn(const std::string& name);

    // M�thode Update() du pion, surcharg�e
    virtual void Update() override;

    // R�cup�ration de la cam�ra
    std::shared_ptr<CameraComponent> GetCamera() const;

private:
    std::shared_ptr<SceneComponent> pSceneComponent;

    std::shared_ptr<CameraComponent> pCameraComponent;
};

