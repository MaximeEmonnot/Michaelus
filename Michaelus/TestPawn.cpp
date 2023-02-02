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
	pCameraComponent->AddRelativeLocation(FVec3D(50.f, 50.f,50.0f));
	CAMERA.SetActiveCamera(pCameraComponent);
}

void TestPawn::Update()
{
	Pawn::Update();
}
