#include "TestPawn.h"

#include "SceneComponent.h"
#include "CameraComponent.h"
#include "CameraManager.h"
#include "Keyboard.h"

TestPawn::TestPawn(const std::string& name)
	:
	Pawn(name)
{
	pSceneComponent = CreateComponent<SceneComponent>();
	pRootComponent = pSceneComponent;

	pCameraComponent = CreateComponent<CameraComponent>();
	pCameraComponent->AttachTo(pSceneComponent);
	pCameraComponent->SetFieldOfView(60.f);
	pCameraComponent->AddRelativeLocation(FVec3D(5.f, 0.f, 5.f));
	CAMERA.SetActiveCamera(pCameraComponent);

	pCameraComponent2 = CreateComponent<CameraComponent>();
	pCameraComponent2->AttachTo(pSceneComponent);
	pCameraComponent2->SetFieldOfView(45.f);
	pCameraComponent2->AddRelativeLocation(FVec3D(0.f, 5.f, 2.5f));

	pCameraComponent3 = CreateComponent<CameraComponent>();
	pCameraComponent3->AttachTo(pSceneComponent);
	pCameraComponent3->SetFieldOfView(15.f);
	pCameraComponent3->AddRelativeLocation(FVec3D(2.5f, -2.5f, -2.5f));

	pCameraComponent4 = CreateComponent<CameraComponent>();
	pCameraComponent4->AttachTo(pSceneComponent);
	pCameraComponent4->SetFieldOfView(90.f);
	pCameraComponent4->AddRelativeLocation(FVec3D(1.f, 0.f, -5.f));

	pCameraComponent5 = CreateComponent<CameraComponent>();
	pCameraComponent5->AttachTo(pSceneComponent);
	pCameraComponent5->SetFieldOfView(50.f);
	pCameraComponent5->AddRelativeLocation(FVec3D(5.f, 0.f, 0.f));
}

void TestPawn::Update()
{
	Pawn::Update();

	if (KBD.KeyIsPressed('A')) CAMERA.SetActiveCamera(pCameraComponent);
	if (KBD.KeyIsPressed('Z')) CAMERA.SetActiveCamera(pCameraComponent2);
	if (KBD.KeyIsPressed('E')) CAMERA.SetActiveCamera(pCameraComponent3);
	if (KBD.KeyIsPressed('R')) CAMERA.SetActiveCamera(pCameraComponent4);
	if (KBD.KeyIsPressed('T')) CAMERA.SetActiveCamera(pCameraComponent5);
}
