#include "DemoPawn.h"

#include "SceneComponent.h"
#include "CameraComponent.h"
#include "CameraManager.h"
#include "DemoControllerMouse.h"
#include "Keyboard.h"
#include "SoundSystem.h"


// Constructeur d�finissant le nom du pion
DemoPawn::DemoPawn(const std::string& name)
	:
	Pawn(name)
{
	pSceneComponent = CreateComponent<SceneComponent>();
	pRootComponent = pSceneComponent;

	pCameraComponent = CreateComponent<CameraComponent>();
	pCameraComponent->AttachTo(pSceneComponent);
	pCameraComponent->SetFieldOfView(60.f);
	pCameraComponent->SetSensitivity(5.f);
	pCameraComponent->AddRelativeLocation(FVec3D(0.f,0.f,0.f));
	CAMERA.SetActiveCamera(pCameraComponent);

	SetController<DemoControllerMouse>();
}

// M�thode Update() du pion, surcharg�e
void DemoPawn::Update()
{
	Pawn::Update();
}

// R�cup�ration de la cam�ra
std::shared_ptr<CameraComponent> DemoPawn::GetCamera() const
{
	return pCameraComponent;
}
