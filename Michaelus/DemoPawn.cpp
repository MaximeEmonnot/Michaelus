#include "DemoPawn.h"

#include "SceneComponent.h"
#include "CameraComponent.h"
#include "CameraManager.h"
#include "Keyboard.h"
#include "LoggerManager.h"
#include "Mouse.h"
#include "SoundSystem.h"
#include "Timer.h"
#include "Window.h"

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
}

void DemoPawn::Update()
{
	Pawn::Update();

	if (KBD.KeyIsPressed('Z')) AddActorLocation(pCameraComponent->GetForwardVector() * DELTATIME);
	if (KBD.KeyIsPressed('D')) AddActorLocation(pCameraComponent->GetRightVector() * DELTATIME);
	if (KBD.KeyIsPressed('S')) AddActorLocation(-pCameraComponent->GetForwardVector() * DELTATIME);
	if (KBD.KeyIsPressed('Q')) AddActorLocation(-pCameraComponent->GetRightVector() * DELTATIME);
	if (KBD.KeyIsPressed('E')) AddActorLocation(pCameraComponent->GetUpVector() * DELTATIME);
	if (KBD.KeyIsPressed('A')) AddActorLocation(-pCameraComponent->GetUpVector() * DELTATIME);

	FVec2D mouseMovement = MOUSE.GetPosition() * DELTATIME * pCameraComponent->GetSensitivity();

	pCameraComponent->AddRelativeRotation(FQuaternion(0.f, MMath::Rad(-mouseMovement.y), MMath::Rad(-mouseMovement.x)));
}
