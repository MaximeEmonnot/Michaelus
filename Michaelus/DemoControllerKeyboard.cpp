#include "DemoControllerKeyboard.h"

#include "DemoControllerMouse.h"
#include "Keyboard.h"
#include "Math.h"
#include "Mouse.h"
#include "Quaternion.h"
#include "Timer.h"
#include "Pawn.h"

DemoControllerKeyboard::DemoControllerKeyboard(Pawn& pawn)
	:
	Controller(pawn)
{
}

void DemoControllerKeyboard::Update()
{
	Controller::Update();

	// Position
	if (KBD.KeyIsPressed('Z')) GetPawn().AddActorLocation(GetPawn().GetActorForwardVector() * DELTATIME);
	if (KBD.KeyIsPressed('D')) GetPawn().AddActorLocation(GetPawn().GetActorRightVector() * DELTATIME);
	if (KBD.KeyIsPressed('S')) GetPawn().AddActorLocation(-GetPawn().GetActorForwardVector() * DELTATIME);
	if (KBD.KeyIsPressed('Q')) GetPawn().AddActorLocation(-GetPawn().GetActorRightVector() * DELTATIME);
	if (KBD.KeyIsPressed('E')) GetPawn().AddActorLocation(GetPawn().GetActorUpVector() * DELTATIME);
	if (KBD.KeyIsPressed('A')) GetPawn().AddActorLocation(-GetPawn().GetActorUpVector() * DELTATIME);

	// Rotation
	const float angle = 0.5f;
	if (KBD.KeyIsPressed('O')) GetPawn().AddActorRotation(FQuaternion(GetPawn().GetActorRightVector(), MMath::Rad(angle)));
	if (KBD.KeyIsPressed('K')) GetPawn().AddActorRotation(FQuaternion(GetPawn().GetActorUpVector(), MMath::Rad(angle)));
	if (KBD.KeyIsPressed('L')) GetPawn().AddActorRotation(FQuaternion(GetPawn().GetActorRightVector(), -MMath::Rad(angle)));
	if (KBD.KeyIsPressed('M')) GetPawn().AddActorRotation(FQuaternion(GetPawn().GetActorUpVector(), -MMath::Rad(angle)));
	if (KBD.KeyIsPressed('I')) GetPawn().AddActorRotation(FQuaternion(GetPawn().GetActorForwardVector(), MMath::Rad(angle)));
	if (KBD.KeyIsPressed('P')) GetPawn().AddActorRotation(FQuaternion(GetPawn().GetActorForwardVector(), -MMath::Rad(angle)));

	// Switch to DemoControllerMouse
	if (MOUSE.Read() == Mouse::EventType::MRelease) GetPawn().SetController<DemoControllerMouse>();
}
