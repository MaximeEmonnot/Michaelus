#include "VKUniformBuffer.h"

#include "Math.h"
#include "VKDevice.h"
#include "VKSwapChain.h"
#include "Window.h"

VKUniformBuffer::VKUniformBuffer()
{
    CreateUniformBuffers();

    InitializeUniformlBuffers();
}

VKUniformBuffer::~VKUniformBuffer()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(VK_DEVICE.GetDevice(), vkUniformBuffers[i], nullptr);
        vkFreeMemory(VK_DEVICE.GetDevice(), vkUniformBuffersMemory[i], nullptr);
    }}

void VKUniformBuffer::Update(UniformBufferObject ubo, uint32_t currentImage)
{
    ubo.projection[1][1] *= -1.f;
    memcpy(vkUniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

std::vector<VkBuffer> VKUniformBuffer::GetUniformBuffers() const
{
    return vkUniformBuffers;
}

std::vector<void*> VKUniformBuffer::GetUniformBuffersMapped() const
{
    return vkUniformBuffersMapped;
}

void VKUniformBuffer::CreateUniformBuffers()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    vkUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    vkUniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    vkUniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VK_DEVICE.CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vkUniformBuffers[i], vkUniformBuffersMemory[i]);
        vkMapMemory(VK_DEVICE.GetDevice(), vkUniformBuffersMemory[i], 0, bufferSize, 0, &vkUniformBuffersMapped[i]);
    }
}

void VKUniformBuffer::InitializeUniformlBuffers()
{
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.f), MMath::Rad(90.f), glm::vec3(0.f, 0.f, 1.f));
    ubo.view = glm::lookAt(glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
    ubo.projection = glm::perspective(MMath::Rad(45.f), static_cast<float>(WND.GetWidth()) / static_cast<float>(WND.GetHeight()), 0.1f, 10.f);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        Update(ubo, i);
}
