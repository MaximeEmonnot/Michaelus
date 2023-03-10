#include "VKUniformBuffer.h"

#include "CameraComponent.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "Math.h"
#include "VKDevice.h"
#include "VKSwapChain.h"
#include "Window.h"

#include <glm/gtx/quaternion.hpp>

#include "LoggerManager.h"


VKUniformBuffer::VKUniformBuffer()
{
    CreateUniformBuffers();
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
    // MODEL
    const FVec3D modelLocation = modelTransform.location;
    const FQuaternion modelRotation = modelTransform.rotation.GetUnit();
    // CAMERA
    const FVec3D cameraLocation = CAMERA.GetActiveCamera()->GetWorldLocation();
    const FVec3D forwardCameraLocation = cameraLocation + CAMERA.GetActiveCamera()->GetForwardVector();
    // LIGHTS
    DirectionalLight directionalLight = LIGHT_SYSTEM.GetDirectionalLight();
    std::vector<PointLight> pointLights = LIGHT_SYSTEM.GetPointLights();

    UniformBufferObject ubo{};
    glm::mat4 modelRotMatrix = glm::toMat4(glm::quat(modelRotation.w, modelRotation.x, modelRotation.y, modelRotation.z));

    ubo.model = glm::translate(glm::mat4(1.f), glm::vec3(modelLocation.y, modelLocation.x, modelLocation.z)) * modelRotMatrix;
    //ubo.view = glm::translate(glm::mat4(1.f), glm::vec3(cameraLocation.y, cameraLocation.x, cameraLocation.z)) * cameraRotMatrix;
	ubo.view = glm::lookAt(glm::vec3(cameraLocation.y, cameraLocation.x, cameraLocation.z), 
        glm::vec3(forwardCameraLocation.y, forwardCameraLocation.x, forwardCameraLocation.z),
        glm::vec3(0.f, 0.f, 1.f));
	ubo.projection = glm::perspective(MMath::Rad(CAMERA.GetActiveCamera()->GetFieldOfView()), static_cast<float>(WND.GetWidth()) / static_cast<float>(WND.GetHeight()), 0.1f, 1000'00.f);
    ubo.projection[1][1] *= -1.f;
    ubo.viewPosition = glm::vec4(cameraLocation.x, cameraLocation.y, cameraLocation.z, 1.f);
    ubo.directional.direction = {directionalLight.direction.x, directionalLight.direction.y, directionalLight.direction.z, 1.f};
    ubo.directional.color = { directionalLight.color.x, directionalLight.color.y, directionalLight.color.z , directionalLight.intensity };
    ubo.lightIntensities = { 0.02f, 2.f, 5.f, 8.f};
    ubo.numLights = static_cast<int>(pointLights.size());
    for (size_t i = 0; i < pointLights.size(); i++)
    {
        PointLight pointLight = pointLights.at(i);

        ubo.pointLights[i].position = { pointLight.position.x, pointLight.position.y, pointLight.position.z, 1.f };
        ubo.pointLights[i].color = { pointLight.color.x, pointLight.color.y, pointLight.color.z, pointLight.intensity };
    }

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
