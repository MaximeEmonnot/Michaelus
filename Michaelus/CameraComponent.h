#pragma once
#include "Component.h"

/*
 * Classe CameraComponent
 * Component définissant une caméra dans CameraManager
 */
class CameraComponent :
    public Component
{
public:
    // Constructeur définissant l'acteur lié au Component
	CameraComponent(Actor& rOwner);

    // Méthode Update() héritée de Component
    virtual void Update() override;

    // Définition de la sensibilité de la caméra
    void SetSensitivity(float newSensitivity);
    // Récupération de la sensibilité de la caméra
    float GetSensitivity() const;

    // Définition du champ de vision (FOV) de la caméra
    void SetFieldOfView(float newFieldOfView);
    // Récupération du champ de vision (FOV) de la caméra
    float GetFieldOfView() const;

private:
    float fieldOfView = 0.f;
    float sensitivity = 1.f;
};

