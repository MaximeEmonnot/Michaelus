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
    const FVec3D cameraUpVector = CAMERA.GetActiveCamera()->GetUpVector();

    // LIGHTS
    DirectionalLight directionalLight = LIGHT_SYSTEM.GetDirectionalLight();
    std::vector<PointLight> pointLights = LIGHT_SYSTEM.GetPointLights();

    UniformBufferObject ubo{};
    glm::mat4 modelRotMatrix = glm::toMat4(glm::quat(modelRotation.w, modelRotation.x, modelRotation.y, modelRotation.z));
    ubo.model = glm::translate(glm::mat4(1.f), glm::vec3(modelLocation.GetY(), modelLocation.GetX(), modelLocation.GetZ())) * modelRotMatrix;
	ubo.view = glm::lookAt(glm::vec3(cameraLocation.GetY(), cameraLocation.GetX(), cameraLocation.GetZ()), 
        glm::vec3(forwardCameraLocation.GetY(), forwardCameraLocation.GetX(), forwardCameraLocation.GetZ()),
        glm::vec3(cameraUpVector.GetY(), cameraUpVector.GetX(),cameraUpVector.GetZ()));
	ubo.projection = glm::perspective(MMath::Rad(CAMERA.GetActiveCamera()->GetFieldOfView()), static_cast<float>(WND.GetWidth()) / static_cast<float>(WND.GetHeight()), 0.1f, 1000'00.f);
    ubo.projection[1][1] *= -1.f;
    ubo.viewPosition = glm::vec4(cameraLocation.GetX(), cameraLocation.GetY(), cameraLocation.GetZ(), 1.f);
    ubo.directional.direction = {directionalLight.direction.GetX(), directionalLight.direction.GetY(), directionalLight.direction.GetZ(), 1.f};
    ubo.directional.color = { directionalLight.color.GetX(), directionalLight.color.GetY(), directionalLight.color.GetZ() , directionalLight.intensity };
    ubo.lightIntensities = { 0.02f, 2.f, 5.f, 8.f};
    ubo.numLights = static_cast<int>(pointLights.size());
    for (size_t i = 0; i < pointLights.size(); i++)
    {
        PointLight pointLight = pointLights.at(i);

        ubo.pointLights[i].position = { pointLight.position.GetX(), pointLight.position.GetY(), pointLight.position.GetZ(), 1.f };
        ubo.pointLights[i].color = { pointLight.color.GetX(), pointLight.color.GetY(), pointLight.color.GetZ(), pointLight.intensity };
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
