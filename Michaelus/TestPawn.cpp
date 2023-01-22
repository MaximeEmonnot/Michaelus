#include "TestPawn.h"

#include "CameraComponent.h"
#include "CameraManager.h"

TestPawn::TestPawn(const std::string& name)
	:
	Pawn(name)
{
	pCameraComponent = CreateComponent<CameraComponent>();
	pRootComponent = pCameraComponent;
	CAMERA.AddCamera(pCameraComponent);
	CAMERA.SetActiveCamera(pCameraComponent);

	pCameraComponent->SetFieldOfView(60.f);
	pCameraComponent->AddRelativeLocation(FVec3D(2.f, 2.f, 2.f));
}

void TestPawn::Update()
{
	Pawn::Update();
}
