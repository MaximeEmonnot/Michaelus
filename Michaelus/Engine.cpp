#include "Engine.h"

#include "Graphics.h"
#include "LightManager.h"
#include "Mouse.h"
#include "SoundSystem.h"
#include "DemoWorld.h"
#include "Keyboard.h"
#include "Timer.h"
#include "VKDevice.h"
#include "Window.h"
#include "World.h"

// VARIABLES STATIQUES
std::unique_ptr<Engine> Engine::pInstance = nullptr;

// ********* //

// Constructeur
Engine::Engine()
{
	// Initialisation du module Graphique
	GFX;

	// Ajout des diff�rentes sc�nes
	AddWorld<DemoWorld>();

	// D�marrage de la sc�ne courante
	pCurrentWorld->BeginPlay();
}

// M�thode du patron de conception Singleton
Engine& Engine::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Engine>();
	return *pInstance;
}

// R�cup�ration du monde courant
std::shared_ptr<World> Engine::GetCurrentWorld() const
{
	return pCurrentWorld;
}

// Ex�cution principale du moteur
void Engine::Run()
{
	// Mise � jour de DeltaTime (voir Timer)
	TICKCLOCK;

	GFX.BeginDraw(); // D�marrage s�quence graphique
	UpdateFrame(); // Mise � jour sc�ne
	GFX.EndDraw(); // Fin s�quence graphique
}

// Mise � jour de la sc�ne courante
void Engine::UpdateFrame() const
{
	pCurrentWorld->Update();

	if (KBD.KeyIsPressed(VK_ESCAPE)) WND.Exit();
}

