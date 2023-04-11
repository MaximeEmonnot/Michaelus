#pragma once
#include "Component.h"

/*
 * Classe CameraComponent
 * Component d�finissant une cam�ra dans CameraManager
 */
class CameraComponent :
    public Component
{
public:
    // Constructeur d�finissant l'acteur li� au Component
	CameraComponent(Actor& rOwner);

    // M�thode Update() h�rit�e de Component
    virtual void Update() override;

    // D�finition de la sensibilit� de la cam�ra
    void SetSensitivity(float newSensitivity);
    // R�cup�ration de la sensibilit� de la cam�ra
    float GetSensitivity() const;

    // D�finition du champ de vision (FOV) de la cam�ra
    void SetFieldOfView(float newFieldOfView);
    // R�cup�ration du champ de vision (FOV) de la cam�ra
    float GetFieldOfView() const;

private:
    float fieldOfView = 0.f;
    float sensitivity = 1.f;
};

