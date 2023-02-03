#include "TestPawn.h"

#include "SceneComponent.h"
#include "CameraComponent.h"
#include "CameraManager.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SoundSystem.h"
#include "Timer.h"
#include "Window.h"

TestPawn::TestPawn(const std::string& name)
	:
	Pawn(name)
{
	pSceneComponent = CreateComponent<SceneComponent>();
	pRootComponent = pSceneComponent;

	pCameraComponent = CreateComponent<CameraComponent>();
	pCameraComponent->AttachTo(pSceneComponent);
	pCameraComponent->SetFieldOfView(60.f);
	pCameraComponent->AddRelativeLocation(FVec3D(0.f,0.f,0.f));
	CAMERA.SetActiveCamera(pCameraComponent);
}

void TestPawn::Update()
{
	Pawn::Update();

	if (KBD.KeyIsPressed('Z')) AddActorLocation(FVec3D(1.f, 0.f, 0.f) * DELTATIME);
	if (KBD.KeyIsPressed('D')) AddActorLocation(FVec3D(0.f, 1.f, 0.f) * DELTATIME);
	if (KBD.KeyIsPressed('S')) AddActorLocation(FVec3D(-1.f, 0.f, 0.f) * DELTATIME);
	if (KBD.KeyIsPressed('Q')) AddActorLocation(FVec3D(0.f, -1.f, 0.f) * DELTATIME);
	if (KBD.KeyIsPressed('A')) AddActorLocation(FVec3D(0.f, 0.f, -1.f) * DELTATIME);
	if (KBD.KeyIsPressed('E')) AddActorLocation(FVec3D(0.f, 0.f, 1.f) * DELTATIME);

}
