#include <Windows.h>
#include <vulkan/vulkan.h>
#include <iostream>

#include "EngineException.h"
#include "Engine.h"
#include "Graphics.h"
#include "Window.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		while (WND.ProcessMessages()) ENGINE.Run();
		GFX.Destroy();
	}
	catch(EngineException& e)
	{
		SHOW_MESSAGE("Vulkan 3D Engine - Engine Exception", e.what())
	}
	catch(std::exception& e)
	{
		SHOW_MESSAGE("Vulkan 3D Engine - Standard Library Exception", e.what())
	}
	catch(...)
	{
		SHOW_MESSAGE("Vulkan 3D Engine - Unknown Exception", "An unknown exception has been caught.");
	}

	return 0;
}
