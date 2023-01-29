#include "VKUniformBuffer.h"

#include "CameraComponent.h"
#include "CameraManager.h"
#include "Math.h"
#include "VKDevice.h"
#include "VKSwapChain.h"
#include "Window.h"


VKUniformBuffer::VKUniformBuffer()
{
    CreateUniformBuffers();
}

VKUniformBuffer::VKUniformBuffer(const VKUniformBuffer& copy)
{
    vkUniformBuffers = copy.vkUniformBuffers;
    vkUniformBuffersMapped = copy.vkUniformBuffersMapped;
    vkUniformBuffersMemory = copy.vkUniformBuffersMemory;
}

VKUniformBuffer::~VKUniformBuffer()
{
}

void VKUniformBuffer::Destroy()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(VK_DEVICE.GetDevice(), vkUniformBuffers[i], nullptr);
        vkFreeMemory(VK_DEVICE.GetDevice(), vkUniformBuffersMemory[i], nullptr);
    }
}

void VKUniformBuffer::Update(const FTransform& modelTransform)
{
    const FVec3D cameraLocation = CAMERA.GetActiveCamera()->GetWorldLocation();
    const FRotator cameraRotation = CAMERA.GetActiveCamera()->GetWorldRotation();

    UniformBufferObject ubo{};

    ubo.model = glm::translate(glm::mat4(1.f), glm::vec3(modelTransform.location.x, modelTransform.location.y, modelTransform.location.z)) * glm::eulerAngleXYZ(modelTransform.rotation.roll, modelTransform.rotation.pitch, modelTransform.rotation.yaw);
    //ubo.view = glm::translate(glm::mat4(1.f), glm::vec3(cameraLocation.x, cameraLocation.y, cameraLocation.z)) * glm::eulerAngleXYX(cameraRotation.roll, cameraRotation.pitch, cameraRotation.yaw);
    //ubo.model = glm::rotate(glm::mat4(1.f), MMath::Rad(90.f), glm::vec3(0.f, 0.f, 1.f));
    ubo.view = glm::lookAt(glm::vec3(cameraLocation.x, cameraLocation.y, cameraLocation.z), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
	ubo.projection = glm::perspective(MMath::Rad(CAMERA.GetActiveCamera()->GetFieldOfView()), static_cast<float>(WND.GetWidth()) / static_cast<float>(WND.GetHeight()), 0.1f, 10.f);
    ubo.projection[1][1] *= -1.f;
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        memcpy(vkUniformBuffersMapped[i], &ubo, sizeof(ubo));
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
