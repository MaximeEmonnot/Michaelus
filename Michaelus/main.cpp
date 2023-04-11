#include <Windows.h>
#include <vulkan/vulkan.h>
#include <iostream>

#include "EngineException.h"
#include "Engine.h"
#include "Graphics.h"
#include "Window.h"

// Point de départ de l'application
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Tant que l'on traite des messages, la boucle principale du moteur est exécutée
		while (WND.ProcessMessages()) ENGINE.Run();
		// A la sortie, on détruit le système graphique
		GFX.Destroy();
	}
	catch(EngineException& e)
	{
		SHOW_MESSAGE("Michaelus 3D Engine - Engine Exception", e.what())
	}
	catch(std::exception& e)
	{
		SHOW_MESSAGE("Michaelus 3D Engine - Standard Library Exception", e.what())
	}
	catch(...)
	{
		SHOW_MESSAGE("Michaelus 3D Engine - Unknown Exception", "An unknown exception has been caught.");
	}

	return 0;
}
