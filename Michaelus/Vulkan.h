#pragma once

#define NOMINMAX
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <Windows.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include "EngineException.h"

#define VK_USE_PLATFORM_WIN32_KHR

// Macro d�finissant simplement la condition d'�chec de Vulkan
#define VK_FAILED(vkResult) vkResult != VK_SUCCESS
// Macro d�finissant simplement une exception graphique
#define GFX_EXCEPTION(note) ENGINE_EXCEPTION("Michaelus 3D Engine - Graphics Engine Exception", note)
