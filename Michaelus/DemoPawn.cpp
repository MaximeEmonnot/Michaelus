#include "DemoPawn.h"

#include "SceneComponent.h"
#include "CameraComponent.h"
#include "CameraManager.h"
#include "DemoControllerKeyboard.h"
#include "DemoControllerMouse.h"
#include "Keyboard.h"
#include "LoggerManager.h"
#include "Mouse.h"
#include "SoundSystem.h"
#include "Timer.h"
#include "Window.h"

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
