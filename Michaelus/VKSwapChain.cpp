#include "VKSwapChain.h"

#include <algorithm>
#include <array>

#include "Graphics.h"
#include "Mesh.h"
#include "VKDevice.h"
#include "Vulkan.h"
#include "Window.h"

VKSwapChain::VKSwapChain()
{
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
    CreateColorResources();
	CreateDepthResources();
	CreateFrameBuffers();
	CreateSynchronizationObjects();
}

VKSwapChain::~VKSwapChain()
{
    
}

void VKSwapChain::Destroy()
{
    vkDeviceWaitIdle(VK_DEVICE.GetDevice());

    CleanUpSwapChain();

    vkDestroyRenderPass(VK_DEVICE.GetDevice(), vkRenderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyFence(VK_DEVICE.GetDevice(), vkInFlightFences[i], nullptr);
        vkDestroySemaphore(VK_DEVICE.GetDevice(), vkRenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(VK_DEVICE.GetDevice(), vkImageAvailableSemaphores[i], nullptr);
    }
}

VkFramebuffer VKSwapChain::GetFrameBuffer(int index) const
{
	return vkSwapChainFrameBuffers.at(index);
}

VkRenderPass VKSwapChain::GetRenderPass() const
{
	return vkRenderPass;
}

VkImageView VKSwapChain::GetImageView(int index) const
{
	return vkSwapChainImageViews.at(index);
}

size_t VKSwapChain::GetImageCount() const
{
	return vkSwapChainImages.size();
}

VkFormat VKSwapChain::GetSwapChainImageFormat() const
{
	return vkSwapChainImageFormat;
}

VkExtent2D VKSwapChain::GetSwapChainExtent() const
{
	return vkSwapChainExtent;
}

uint32_t VKSwapChain::GetWidth() const
{
	return vkSwapChainExtent.width;
}

uint32_t VKSwapChain::GetHeight() const
{
	return vkSwapChainExtent.height;
}

float VKSwapChain::GetExtentAspectRatio() const
{
	return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());
}

VkFormat VKSwapChain::FindDepthFormat()
{
    return VK_DEVICE.FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

void VKSwapChain::BeginDraw()
{
    vkWaitForFences(VK_DEVICE.GetDevice(), 1, &vkInFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(VK_DEVICE.GetDevice(), vkSwapChain, UINT64_MAX, vkImageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
        RecreateSwapChain();
    if (VK_FAILED(result) && result != VK_SUBOPTIMAL_KHR)
        throw GFX_EXCEPTION("Failed to acquire Swap Chain Image!");
}

void VKSwapChain::EndDraw(VkCommandBuffer* commandBuffers, std::vector<std::shared_ptr<Mesh>> meshes)
{
    vkResetFences(VK_DEVICE.GetDevice(), 1, &vkInFlightFences[currentFrame]);

    vkResetCommandBuffer(commandBuffers[currentFrame], 0);

    RecordCommandBuffer(commandBuffers[currentFrame], meshes);

    SubmitCommandBuffers(commandBuffers);
}

void VKSwapChain::RecordCommandBuffer(VkCommandBuffer commandBuffer, std::vector<std::shared_ptr<Mesh>> meshes)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (VK_FAILED(vkBeginCommandBuffer(commandBuffer, &beginInfo)))
        throw std::runtime_error("Failed to begin recording command buffer!");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = GetRenderPass();
    renderPassInfo.framebuffer = vkSwapChainFrameBuffers[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = GetSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.f;
    viewport.y = 0.f;
    viewport.width = static_cast<float>(GetWidth());
    viewport.height = static_cast<float>(GetHeight());
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = GetSwapChainExtent();
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    for (std::shared_ptr<Mesh> mesh : meshes) mesh->Draw(commandBuffer, currentFrame);

    vkCmdEndRenderPass(commandBuffer);

    if (VK_FAILED(vkEndCommandBuffer(commandBuffer)))
        throw GFX_EXCEPTION("Failed to record Command Buffer!");
}

void VKSwapChain::SubmitCommandBuffers(const VkCommandBuffer* pCommandBuffers)
{
    VkSemaphore signalSemaphores[] = { vkRenderFinishedSemaphores[currentFrame] };
    VkSemaphore waitSemaphores[] = { vkImageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &pCommandBuffers[currentFrame];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (VK_FAILED(vkQueueSubmit(VK_DEVICE.GetGraphicsQueue(), 1, &submitInfo, vkInFlightFences[currentFrame])))
        throw GFX_EXCEPTION("Failed to submit Draw Command Buffer!");

    VkSwapchainKHR swapChains[] = { vkSwapChain };

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    VkResult result = vkQueuePresentKHR(VK_DEVICE.GetPresentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) RecreateSwapChain();
    else if (VK_FAILED(result))
        throw GFX_EXCEPTION("Failed to present Swap Chain Image !");

    currentFrame = (++currentFrame) % MAX_FRAMES_IN_FLIGHT;
}

bool VKSwapChain::operator==(const VKSwapChain& rhs) const
{
	return vkSwapChainImageFormat == rhs.vkSwapChainImageFormat &&
		   vkSwapChainDepthFormat == rhs.vkSwapChainDepthFormat;
}

void VKSwapChain::CreateSwapChain()
{
    SwapChainSupportDetails swapChainSupport = VK_DEVICE.GetSwapChainSupport();

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.minImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        imageCount = swapChainSupport.capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = VK_DEVICE.GetSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = VK_DEVICE.FindPhysicalQueueFamilies();
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (VK_FAILED(vkCreateSwapchainKHR(VK_DEVICE.GetDevice(), &createInfo, nullptr, &vkSwapChain)))
        throw GFX_EXCEPTION("Failed to create swap chain!");

    vkGetSwapchainImagesKHR(VK_DEVICE.GetDevice(), vkSwapChain, &imageCount, nullptr);
    vkSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(VK_DEVICE.GetDevice(), vkSwapChain, &imageCount, vkSwapChainImages.data());

    vkSwapChainImageFormat = surfaceFormat.format;
    vkSwapChainExtent = extent;
}

void VKSwapChain::CreateImageViews()
{
    vkSwapChainImageViews.resize(vkSwapChainImages.size());

    for (size_t i = 0; i < vkSwapChainImages.size(); i++)
        vkSwapChainImageViews[i] = VK_DEVICE.CreateImageView(vkSwapChainImages[i], vkSwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

void VKSwapChain::CreateColorResources()
{
    VkFormat colorFormat = vkSwapChainImageFormat;

    vkColorImages.resize(vkSwapChainImages.size());
    vkColorImageMemories.resize(vkSwapChainImages.size());
    vkColorImageViews.resize(vkSwapChainImages.size());
        ;
    for (size_t i = 0; i < vkColorImages.size(); i++) {
        VK_DEVICE.CreateImage(vkSwapChainExtent.width, vkSwapChainExtent.height, 1, VK_DEVICE.GetMSAASamples(), colorFormat,
            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkColorImages[i], vkColorImageMemories[i]);
        vkColorImageViews[i] = VK_DEVICE.CreateImageView(vkColorImages[i], colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }
}

void VKSwapChain::CreateDepthResources()
{
    VkFormat depthFormat = FindDepthFormat();

    vkDepthImages.resize(vkSwapChainImages.size());
    vkDepthImageMemories.resize(vkSwapChainImages.size());
    vkDepthImageViews.resize(vkSwapChainImages.size());

    for (size_t i = 0; i < vkDepthImages.size(); i++) {

        VK_DEVICE.CreateImage(vkSwapChainExtent.width, vkSwapChainExtent.height, 1, VK_DEVICE.GetMSAASamples(), depthFormat,
            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkDepthImages[i], vkDepthImageMemories[i]);
        vkDepthImageViews[i] = VK_DEVICE.CreateImageView(vkDepthImages[i], depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

        VK_DEVICE.TransitionImageLayout(vkDepthImages[i], depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
    }
}

void VKSwapChain::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = vkSwapChainImageFormat;
    colorAttachment.samples = VK_DEVICE.GetMSAASamples();
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = vkSwapChainImageFormat;
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 2;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_DEVICE.GetMSAASamples();
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subPass{};
    subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPass.colorAttachmentCount = 1;
    subPass.pColorAttachments = &colorAttachmentRef;
    subPass.pDepthStencilAttachment = &depthAttachmentRef;
    subPass.pResolveAttachments = &colorAttachmentResolveRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subPass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (VK_FAILED(vkCreateRenderPass(VK_DEVICE.GetDevice(), &renderPassInfo, nullptr, &vkRenderPass)))
        throw GFX_EXCEPTION("Failed to create Render Pass!");
}

void VKSwapChain::CreateFrameBuffers()
{
    vkSwapChainFrameBuffers.resize(vkSwapChainImageViews.size());

    for (size_t i = 0; i < vkSwapChainImageViews.size(); i++)
    {
        std::array<VkImageView, 3> attachments = {
            vkColorImageViews[i],
            vkDepthImageViews[i],
            vkSwapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vkRenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = vkSwapChainExtent.width;
        framebufferInfo.height = vkSwapChainExtent.height;
        framebufferInfo.layers = 1;

        if (VK_FAILED(vkCreateFramebuffer(VK_DEVICE.GetDevice(), &framebufferInfo, nullptr, &vkSwapChainFrameBuffers[i])))
            throw GFX_EXCEPTION("Failed to create Framebuffer!");
    }
}

void VKSwapChain::CreateSynchronizationObjects()
{
    vkImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    vkRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    vkInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (VK_FAILED(vkCreateSemaphore(VK_DEVICE.GetDevice(), &semaphoreInfo, nullptr, &vkImageAvailableSemaphores[i])) ||
            VK_FAILED(vkCreateSemaphore(VK_DEVICE.GetDevice(), &semaphoreInfo, nullptr, &vkRenderFinishedSemaphores[i])) ||
            VK_FAILED(vkCreateFence(VK_DEVICE.GetDevice(), &fenceInfo, nullptr, &vkInFlightFences[i])))
            throw GFX_EXCEPTION("Failed to create semaphores!");
    }
}

void VKSwapChain::CleanUpSwapChain()
{
    for (size_t i = 0; i < vkColorImages.size(); i++)
    {
        vkDestroyImageView(VK_DEVICE.GetDevice(), vkColorImageViews[i], nullptr);
        vkDestroyImage(VK_DEVICE.GetDevice(), vkColorImages[i], nullptr);
        vkFreeMemory(VK_DEVICE.GetDevice(), vkColorImageMemories[i], nullptr);
    }

    for (size_t i = 0; i < vkDepthImages.size(); i++)
    {
        vkDestroyImageView(VK_DEVICE.GetDevice(), vkDepthImageViews[i], nullptr);
        vkDestroyImage(VK_DEVICE.GetDevice(), vkDepthImages[i], nullptr);
        vkFreeMemory(VK_DEVICE.GetDevice(), vkDepthImageMemories[i], nullptr);
    }

    for (auto vkFrameBuffer : vkSwapChainFrameBuffers)
        vkDestroyFramebuffer(VK_DEVICE.GetDevice(), vkFrameBuffer, nullptr);
    vkSwapChainFrameBuffers.clear();

    for (const auto imageView : vkSwapChainImageViews)
        vkDestroyImageView(VK_DEVICE.GetDevice(), imageView, nullptr);
    vkSwapChainImageViews.clear();

    vkDestroySwapchainKHR(VK_DEVICE.GetDevice(), vkSwapChain, nullptr);
}

void VKSwapChain::RecreateSwapChain()
{
    vkDeviceWaitIdle(VK_DEVICE.GetDevice());

    CleanUpSwapChain();

    CreateSwapChain();
    CreateImageViews();
    CreateColorResources();
    CreateDepthResources();
    CreateFrameBuffers();
}

VkSurfaceFormatKHR VKSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) return availableFormat;

    return availableFormats.at(0); ;
}

VkPresentModeKHR VKSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) return availablePresentMode;

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VKSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) return capabilities.currentExtent;

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(WND.GetWidth()),
        static_cast<uint32_t>(WND.GetHeight())
    };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}
