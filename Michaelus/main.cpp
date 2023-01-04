#include <Windows.h>
#include <vulkan/vulkan.h>
#include <iostream>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Test";
	appInfo.pEngineName = "MichaelusEngine";

	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;

	VkInstance instance;

	VkResult result = vkCreateInstance(&instanceInfo, 0, &instance);
	if (result == VK_SUCCESS)
		OutputDebugStringA("Vulkan works!\n");

	return 0;
}