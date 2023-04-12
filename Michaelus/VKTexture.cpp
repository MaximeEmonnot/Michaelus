#include "VKTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "VKDevice.h"

// Constructeur définissant la Texture
VKTexture::VKTexture(const std::string& texturePath)
{
	CreateTextureImage(texturePath);
	CreateTextureImageView();
	CreateTextureSampler();
}

// Destructeur réel pour contrôler la libération de mémoire
void VKTexture::Destroy()
{
    vkDestroySampler(VK_DEVICE.GetDevice(), vkTextureSampler, nullptr);
    vkDestroyImageView(VK_DEVICE.GetDevice(), vkTextureImageView, nullptr);
    vkDestroyImage(VK_DEVICE.GetDevice(), vkTextureImage, nullptr);
    vkFreeMemory(VK_DEVICE.GetDevice(), vkTextureImageMemory, nullptr);
}

// Récupération de l'Image (Texture)
VkImage VKTexture::GetTextureImage() const
{
    return vkTextureImage;
}

// Récupération de la vue vers l'Image
VkImageView VKTexture::GetTextureImageView() const
{
    return vkTextureImageView;
}

// Récupération du Sampler de la Texture
VkSampler VKTexture::GetTextureSampler() const
{
    return vkTextureSampler;
}

// Création de l'Image (Texture)
void VKTexture::CreateTextureImage(const std::string& texturePath)
{
    // Chargement de la Texture
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;
    vkMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
    if (!pixels)
        throw GFX_EXCEPTION("Failed to load Texture Image!");

    // Création du Buffer de la Texture
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VK_DEVICE.CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    void* data;
    vkMapMemory(VK_DEVICE.GetDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(VK_DEVICE.GetDevice(), stagingBufferMemory);

    stbi_image_free(pixels);
    // Création de l'Image
    VK_DEVICE.CreateImage(texWidth, texHeight, vkMipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vkTextureImage, vkTextureImageMemory);

    // Transition du Buffer de la Texture vers l'Image, et MipMapping de la Texture
    VK_DEVICE.TransitionImageLayout(vkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vkMipLevels);
    VK_DEVICE.CopyBufferToImage(stagingBuffer, vkTextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    VK_DEVICE.GenerateMipMaps(vkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, vkMipLevels);

    // Libération de la mémoire des Buffers temporaires
    vkDestroyBuffer(VK_DEVICE.GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VK_DEVICE.GetDevice(), stagingBufferMemory, nullptr);
}

// Création de la vue vers l'Image
void VKTexture::CreateTextureImageView()
{
    vkTextureImageView = VK_DEVICE.CreateImageView(vkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, vkMipLevels);
}

// Création du Sampler de la Texture
void VKTexture::CreateTextureSampler()
{
    // Définition des paramètres du Sampler
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(VK_DEVICE.GetPhysicalDevice(), &properties);
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = static_cast<float>(vkMipLevels);
    samplerInfo.mipLodBias = 0.0f;

    // Création du Sampler
    if (VK_FAILED(vkCreateSampler(VK_DEVICE.GetDevice(), &samplerInfo, nullptr, &vkTextureSampler)))
        throw GFX_EXCEPTION("Failed to create Texture Sampler!");
}
