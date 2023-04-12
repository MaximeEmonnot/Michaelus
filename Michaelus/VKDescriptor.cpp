#include "VKDescriptor.h"

#include <array>

#include "Graphics.h"
#include "VKDevice.h"
#include "VKSwapChain.h"
#include "VKTexture.h"
#include "VKUniformBuffer.h"
#include "Vulkan.h"

// Constructeur définissant la texture à sampler, ainsi que l'UniformBuffer qui lui est associé
VKDescriptor::VKDescriptor(const VKTexture& texture, const VKUniformBuffer& uniformBuffer)
{
    CreateDescriptorSetLayout();
    CreateDescriptorPool();
    CreateDescriptorSets(texture, uniformBuffer);
}

// Destructeur réel pour contrôler la libération de mémoire
void VKDescriptor::Destroy()
{
    vkDestroyDescriptorPool(VK_DEVICE.GetDevice(), vkDescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(VK_DEVICE.GetDevice(), vkDescriptorSetLayout, nullptr);
}

// Récupération du Layout du Descriptor
VkDescriptorSetLayout* VKDescriptor::GetDescriptorSetLayout()
{
    return &vkDescriptorSetLayout;
}

// Récupération des Sets de Descriptor
std::vector<VkDescriptorSet> VKDescriptor::GetDescriptorSets() const
{
    return vkDescriptorSets;
}

void VKDescriptor::CreateDescriptorSetLayout()
{
    // Description de l'Uniform Buffer
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    // Description du Sampler de Texture
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    // Création des Layouts pour les deux descriptions précédentes
    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (VK_FAILED(vkCreateDescriptorSetLayout(VK_DEVICE.GetDevice(), &layoutInfo, nullptr, &vkDescriptorSetLayout)))
        throw GFX_EXCEPTION("Failed to create Descriptor Set Layout!");
}

void VKDescriptor::CreateDescriptorPool()
{
    // Création d'une Pool de descriptions, pour Uniform Buffer et Texture Sampler
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    // Définition de la Pool de description, et création
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (VK_FAILED(vkCreateDescriptorPool(VK_DEVICE.GetDevice(), &poolInfo, nullptr, &vkDescriptorPool)))
        throw GFX_EXCEPTION("Failed to create Descriptor Pool!");
}

void VKDescriptor::CreateDescriptorSets(const VKTexture& texture, const VKUniformBuffer& uniformBuffer)
{
    // Allocation de la mémoire pour les DescriptorSets
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, vkDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = vkDescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    vkDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

    if (VK_FAILED(vkAllocateDescriptorSets(VK_DEVICE.GetDevice(), &allocInfo, vkDescriptorSets.data())))
        throw GFX_EXCEPTION("Failed to allocate Descriptor Sets!");

    // Création des DescriptorSets
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        // Définition de l'UniformBuffer
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffer.GetUniformBuffers().at(i);
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        // Définition du Texture Sampler
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture.GetTextureImageView();
        imageInfo.sampler = texture.GetTextureSampler();

        // Description finale pour l'écriture sur l'Uniform Buffer et le Texture Sampler
        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = vkDescriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = vkDescriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(VK_DEVICE.GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}
