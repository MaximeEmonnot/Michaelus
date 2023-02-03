#include "Engine.h"

#include "Graphics.h"
#include "LightManager.h"
#include "LoggerManager.h"
#include "Mouse.h"
#include "SoundSystem.h"
#include "TestActor.h"
#include "TestPawn.h"
#include "Timer.h"
#include "VKDevice.h"
#include "World.h"

std::unique_ptr<Engine> Engine::pInstance = nullptr;

Engine::Engine()
{
	GFX;

	AddWorld<World>();

	pCurrentWorld->BeginPlay();
}

Engine& Engine::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Engine>();
	return *pInstance;
}

std::shared_ptr<World> Engine::GetCurrentWorld() const
{
	return pCurrentWorld;
}

void Engine::Run()
{
	TICKCLOCK;

	GFX.BeginDraw();
	UpdateFrame();
	GFX.EndDraw();
}

void Engine::UpdateFrame() const
{
	pCurrentWorld->Update();
}

