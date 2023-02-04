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
	pCameraComponent->SetSensitivity(0.8f);
	pCameraComponent->AddRelativeLocation(FVec3D(0.f,0.f,0.f));
	CAMERA.SetActiveCamera(pCameraComponent);
}

void DemoPawn::Update()
{
	Pawn::Update();

	if (KBD.KeyIsPressed('Z')) AddActorLocation(GetActorForwardVector() * DELTATIME);
	if (KBD.KeyIsPressed('D')) AddActorLocation(GetActorRightVector() * DELTATIME);
	if (KBD.KeyIsPressed('S')) AddActorLocation(-GetActorForwardVector() * DELTATIME);
	if (KBD.KeyIsPressed('Q')) AddActorLocation(-GetActorRightVector() * DELTATIME);
	if (KBD.KeyIsPressed('E')) AddActorLocation(GetActorUpVector() * DELTATIME);
	if (KBD.KeyIsPressed('A')) AddActorLocation(-GetActorUpVector() * DELTATIME);

	FVec2D mouseMovement = MOUSE.GetPosition() * 0.01f;

	AddActorRotation(FQuaternion(0.f, MMath::Rad(-mouseMovement.y), MMath::Rad(-mouseMovement.x)) * DELTATIME * pCameraComponent->GetSensitivity());
}
