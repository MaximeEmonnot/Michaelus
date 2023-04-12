#include "VKDevice.h"

#include "Vulkan.h"
#include "Graphics.h"
#include "Window.h"

#include <set>

// VARIABLES STATIQUES
std::unique_ptr<VKDevice> VKDevice::pInstance = nullptr;

// ********* //
/*
 * NOTE : Cette classe est tr�s longue et prends BEAUCOUP de temps pour �tre analys�e et comprise dans son int�gralit�.
 * Si vous �tes curieux de comprendre le fonctionnement de Vulkan, continuez, sinon il est pr�f�rable de regarder les autres fichiers
 */

// Cr�ation du syst�me de debug de Vulkan
// Vulkan poss�de son propre syst�me de debug appel� ValidationLayer, qui d�crit toutes les erreurs possibles lors des appels � la carte graphique
// Ces erreurs sont tr�s longues mais permettent de guider le d�veloppeur pour le d�bogage des m�thodes Vulkan
static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    if (const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")))
        return func(instance, pCreateInfos, pAllocator, pDebugMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

// Destruction du syst�me de debug Vulkan
static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    if (const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT")))
        func(instance, debugMessenger, pAllocator);
}

// Constructeur
// Initialisation compl�te du Syst�me Vulkan
VKDevice::VKDevice()
{
	CreateInstance();
	SetupDebugMessenger();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateCommandPool();
}

// M�thode du patron de conception Singleton
VKDevice& VKDevice::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<VKDevice>();
	return *pInstance;
}

// Destructeur r�el pour contr�ler la lib�ration de m�moire
// EXTREMEMENT IMPORTANT : Le device doit �tre d�truit en dernier pour �viter toute fuite de m�moire
void VKDevice::Destroy()
{
    vkDestroyCommandPool(vkDevice, vkCommandPool, nullptr);

    vkDestroyDevice(vkDevice, nullptr);

    if (bEnableValidationLayers)
        DestroyDebugUtilsMessengerEXT(vkInstance, vkDebugMessenger, nullptr);

    vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
    vkDestroyInstance(vkInstance, nullptr);
}

// R�cup�ration du Device Vulkan
VkDevice VKDevice::GetDevice() const
{
    return vkDevice;
}

// R�cup�ration du Device Physique (Carte graphique)
VkPhysicalDevice VKDevice::GetPhysicalDevice() const
{
    return vkPhysicalDevice;
}

// R�cup�ration de la surface de rendu
VkSurfaceKHR VKDevice::GetSurface() const
{
    return vkSurface;
}

// R�cup�ration de la file graphique
VkQueue VKDevice::GetGraphicsQueue() const
{
    return vkGraphicsQueue;
}

// R�cup�ration de la file de pr�sentation
VkQueue VKDevice::GetPresentQueue() const
{
    return vkPresentQueue;
}

// R�cup�ration de la Pool de commandes
VkCommandPool VKDevice::GetCommandPool() const
{
    return vkCommandPool;
}

// R�cup�ration des Samples MSAA (Multi sampling des textures)
VkSampleCountFlagBits VKDevice::GetMSAASamples() const
{
    return vkMSAASamples;
}

// R�cup�ration des d�tails pour le support du SwapChain
SwapChainSupportDetails VKDevice::GetSwapChainSupport()
{
    return QuerySwapChainSupport(vkPhysicalDevice, vkSurface);
}

// R�cup�ration du type de m�moire pour le Device Physique
uint32_t VKDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memoryProperties);

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) return i;
    }
    throw GFX_EXCEPTION("Failed to find suitable Memory Type!");
}

// R�cup�ration des propri�t�s de file graphique et de pr�sentation pour le Device Physique
QueueFamilyIndices VKDevice::FindPhysicalQueueFamilies()
{
    return FindQueueFamilies(vkPhysicalDevice);
}

// R�cup�ration du format support� pour le Device Physique
VkFormat VKDevice::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
{
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) return format;
        if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) return format;
    }
    throw GFX_EXCEPTION("Failed to find supported format!");
}

// Cr�ation de l'Instance Vulkan
void VKDevice::CreateInstance()
{
    // D�finition des informations pour l'application
    // Dans notre cas, il s'agit du moteur Michaelus
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Michaelus";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Michaelus 3D Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    // D�finition des extensions Vulkan
    const char* extensions[] = {
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    VK_KHR_SURFACE_EXTENSION_NAME
    };

    // D�finition des informations de l'instance
    // Exemple : L'application, les extension, le syst�me de debug...
    VkInstanceCreateInfo instanceInfos{};
    instanceInfos.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfos.pApplicationInfo = &appInfo;
    instanceInfos.ppEnabledExtensionNames = extensions;
    instanceInfos.enabledExtensionCount = static_cast<uint32_t>(std::size(extensions));
    if (bEnableValidationLayers && CheckValidationLayerSupport()) {
        instanceInfos.ppEnabledLayerNames = vkValidationLayers.data();
        instanceInfos.enabledLayerCount = static_cast<uint32_t>(vkValidationLayers.size());
    }
    else instanceInfos.enabledLayerCount = 0;

    // Cr�ation de l'instance
    if (VK_FAILED(vkCreateInstance(&instanceInfos, nullptr, &vkInstance)))
        throw GFX_EXCEPTION("An exception has been caught during the Vulkan Instance Creation");
}

// D�finition du syst�me de Debug Vulkan
void VKDevice::SetupDebugMessenger()
{
    // Si ce syst�me est d�sactiv� par d�faut, inutile d'aller plus loin
    if (!bEnableValidationLayers) return;

    // D�finition du syst�me de Debug
    VkDebugUtilsMessengerCreateInfoEXT debugInfos{};
    debugInfos.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugInfos.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugInfos.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugInfos.pfnUserCallback = DebugCallback;
    debugInfos.pUserData = nullptr;

    // Cr�ation du syst�me de Debug
    if (VK_FAILED(CreateDebugUtilsMessengerEXT(vkInstance, &debugInfos, nullptr, &vkDebugMessenger)))
        throw GFX_EXCEPTION("An exception has been caught during the Debug Messenger Creation");
}

// S�lection d'un Device Physique
void VKDevice::PickPhysicalDevice()
{
    // On �num�re l'ensemble des processeurs graphiques pr�sents sur l'ordinateur
    // Exemple : Un ordinateur avec un CPU et un GPU aura deux processeurs graphiques, s'il y a deux cartes graphiques le syst�me le saura �galement
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
    if (deviceCount == 0)
        throw GFX_EXCEPTION("Failed to find GPUs with Vulkan support!");
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    // Pour tous les processeurs graphiques, on cherche le processeur le plus ad�quat
    // Tr�s important car ce processeur d�finira la qualit� d'image de l'application, avec notamment les Samples MSAA
    for (const auto& device : devices)
    {
        if (IsDeviceSuitable(device)) {
            vkPhysicalDevice = device;
            vkMSAASamples = GetMaxUsableSampleCount();
            break;
        }
    }

    // Si au final aucun processeur graphique n'est ad�quat, on l�ve une exception
    if (vkPhysicalDevice == VK_NULL_HANDLE)
        throw GFX_EXCEPTION("Failed to find a suitable GPU!");
}

// Cr�ation d'un Device Logique
void VKDevice::CreateLogicalDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(vkPhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
    float queuePriority = 1.f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.sampleRateShading = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(vkDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = vkDeviceExtensions.data();
    createInfo.pNext = nullptr;

    if (VK_FAILED(vkCreateDevice(vkPhysicalDevice, &createInfo, nullptr, &vkDevice)))
        throw GFX_EXCEPTION("Failed to create logical device!");

    vkGetDeviceQueue(vkDevice, indices.graphicsFamily.value(), 0, &vkGraphicsQueue);
    vkGetDeviceQueue(vkDevice, indices.presentFamily.value(), 0, &vkPresentQueue);
}

// Cr�ation de la surface de rendu : Dans notre cas nous r�alisons un rendu dans une fen�tre g�r�e par l'API Win32
void VKDevice::CreateSurface()
{
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = WND.GetHWND();
    createInfo.hinstance = WND.GetHInstance();

    if (VK_FAILED(vkCreateWin32SurfaceKHR(vkInstance, &createInfo, nullptr, &vkSurface)))
        throw GFX_EXCEPTION("Failed to create window surface!");
}

// Cr�ation de la CommandPool : Important pour permettre l'affichage des objets dans la sc�ne
void VKDevice::CreateCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(vkPhysicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (VK_FAILED(vkCreateCommandPool(vkDevice, &poolInfo, nullptr, &vkCommandPool)))
        throw GFX_EXCEPTION("Failed to create command pool");
}

// M�thode v�rification d'un processeur graphique
// On v�rifie si le processeur supporte les extensions n�cessaires, la famille de file graphique etc
bool VKDevice::IsDeviceSuitable(VkPhysicalDevice device)
{
    const QueueFamilyIndices indices = FindQueueFamilies(device);

    const bool bExtensionsSupported = CheckDeviceExtensionSupport(device);

    bool bSwapChainAdequate = false;
    if (bExtensionsSupported)
    {
        const SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, vkSurface);
        bSwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.IsComplete() && bExtensionsSupported && bSwapChainAdequate && supportedFeatures.samplerAnisotropy;
}

// M�thode v�rification support des Validation Layers (Debug Vulkan)
bool VKDevice::CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    bool layerFound = false;
    for (const std::string layerName : vkValidationLayers)
    {
        for (const auto& layerProperties : availableLayers)
        {
            if (layerName == layerProperties.layerName)
            {
                layerFound = true;
                break;
            }
        }
    }
    return layerFound;
}

// R�cup�ration des propri�t�s de file pour la file graphique et de pr�sentation
QueueFamilyIndices VKDevice::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vkSurface, &presentSupport);
        if (presentSupport) indices.presentFamily = i;
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphicsFamily = i;
        if (indices.IsComplete()) break;
        i++;
    }

    return indices;
}

// V�rification du support des extensions pour un Device Physique
bool VKDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtension(vkDeviceExtensions.begin(), vkDeviceExtensions.end());

    for (const auto& extension : availableExtensions) requiredExtension.erase(extension.extensionName);

    return requiredExtension.empty();
}

// R�cup�ration des d�tails de support pour le SwapChain
SwapChainSupportDetails VKDevice::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

bool VKDevice::HasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

// Cr�ation centralis�e d'un Buffer
void VKDevice::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    // D�finition des informations du Buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    // Cr�ation du Buffer
    if (VK_FAILED(vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &buffer)))
        throw GFX_EXCEPTION("Failed to create Buffer!");

    // Allocation de la m�moire pour le Buffer
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(vkDevice, buffer, &memoryRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties);

    if (VK_FAILED(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &bufferMemory)))
        throw GFX_EXCEPTION("Failed to allocate Buffer Memory!");

    // Lien du Buffer avec sa m�moire
    vkBindBufferMemory(vkDevice, buffer, bufferMemory, 0);
}

// Copie centralis�e d'un Buffer
void VKDevice::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    // D�marrage d'un CommandBuffer
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    // Copie d'un Buffer dans le CommandBuffer
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    // Fin du CommandBuffer
    EndSingleTimeCommands(commandBuffer);
}

// Copie centralis�e d'un Buffer vers une Image
void VKDevice::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    // D�marrage d'un CommandBuffer
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    EndSingleTimeCommands(commandBuffer);
}

// D�marrage d'un CommandBuffer pour le rendu
VkCommandBuffer VKDevice::BeginSingleTimeCommands()
{
    // Param�tres pour l'allocation du CommandBuffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(vkDevice, &allocInfo, &commandBuffer);

    // Param�tres pour le d�marrage du CommandBuffer
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

// Fin d'un CommandBuffer pour le rendu
void VKDevice::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    // Fin du CommandBuffer
    vkEndCommandBuffer(commandBuffer);

    // Param�tres pour l'enregistrement de la file graphique
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(vkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(vkGraphicsQueue);

    // Lib�ration de la m�moire du CommandBuffer
    vkFreeCommandBuffers(vkDevice, vkCommandPool, 1, &commandBuffer);
}

// Cr�ation centralis�e d'une Image
void VKDevice::CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    // D�finition des param�tres de l'Image
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = numSamples;

    // Cr�ation de l'Image
    if (VK_FAILED(vkCreateImage(vkDevice, &imageInfo, nullptr, &image)))
        throw GFX_EXCEPTION("Failed to create Image!");

    // R�cup�ration des pr�requis de l'Image
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vkDevice, image, &memRequirements);

    // Allocation de la m�moire de l'Image
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (VK_FAILED(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &imageMemory)))
        throw GFX_EXCEPTION("Failed to allocate Image Memory!");

    // Lien entre l'Image et sa m�moire
    vkBindImageMemory(vkDevice, image, imageMemory, 0);
}

// Cr�ation centralis�e d'une vue vers une Image
VkImageView VKDevice::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
{
    // D�finition des param�tres de la vue
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    // Cr�ation de la vue
    VkImageView imageView;
    if (VK_FAILED(vkCreateImageView(vkDevice, &viewInfo, nullptr, &imageView)))
        throw GFX_EXCEPTION("Failed to create texture image view!");

    return imageView;
}

// Transition centralis�e d'un Layout d'Image
void VKDevice::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        if (HasStencilComponent(format)) barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    else barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

    }
    else throw GFX_EXCEPTION("Unsupported layout transition!");

    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    EndSingleTimeCommands(commandBuffer);
}

// G�n�ration du MipMapping pour les images (Textures)
void VKDevice::GenerateMipMaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
{
    // R�cup�ration des priorit�s pour le MipMapping
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, format, &formatProperties);
    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        throw GFX_EXCEPTION("Texture image does not support linear blitting!");

    // D�marrage d'un CommandBuffer
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    // Cr�ation d'une barri�re pour la m�moire de l'image
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    for (uint32_t i = 1; i < mipLevels; i++)
    {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    EndSingleTimeCommands(commandBuffer);
}

// R�cup�ration du nombre de Sample maximum pour le MSAA
// Ce nombre maximum d�pend de la carte graphique
VkSampleCountFlagBits VKDevice::GetMaxUsableSampleCount()
{
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &physicalDeviceProperties);

    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

    if (counts & VK_SAMPLE_COUNT_64_BIT) return VK_SAMPLE_COUNT_64_BIT;
    if (counts & VK_SAMPLE_COUNT_32_BIT) return VK_SAMPLE_COUNT_32_BIT;
    if (counts & VK_SAMPLE_COUNT_16_BIT) return VK_SAMPLE_COUNT_16_BIT;
    if (counts & VK_SAMPLE_COUNT_8_BIT) return VK_SAMPLE_COUNT_8_BIT;
    if (counts & VK_SAMPLE_COUNT_4_BIT) return VK_SAMPLE_COUNT_4_BIT;
    if (counts & VK_SAMPLE_COUNT_2_BIT) return VK_SAMPLE_COUNT_2_BIT;
    return VK_SAMPLE_COUNT_1_BIT;
}

// M�thode Callback du syst�me de Debug de Vulkan
// Cette m�thode sert juste � lever l'exception de Vulkan, pour la traiter par le syst�me de Michaelus
VkBool32 VKDevice::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        const char* message = pCallbackData->pMessage;
        OutputDebugStringA(message);
        throw GFX_EXCEPTION(message);
    }

    return VK_FALSE;
}
