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

	// Ajout des différentes scènes
	AddWorld<DemoWorld>();

	// Démarrage de la scène courante
	pCurrentWorld->BeginPlay();
}

// Méthode du patron de conception Singleton
Engine& Engine::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Engine>();
	return *pInstance;
}

// Récupération du monde courant
std::shared_ptr<World> Engine::GetCurrentWorld() const
{
	return pCurrentWorld;
}

// Exécution principale du moteur
void Engine::Run()
{
	// Mise à jour de DeltaTime (voir Timer)
	TICKCLOCK;

	GFX.BeginDraw(); // Démarrage séquence graphique
	UpdateFrame(); // Mise à jour scène
	GFX.EndDraw(); // Fin séquence graphique
}

// Mise à jour de la scène courante
void Engine::UpdateFrame() const
{
	pCurrentWorld->Update();

	if (KBD.KeyIsPressed(VK_ESCAPE)) WND.Exit();
}

