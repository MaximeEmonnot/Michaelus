#include "VKModel.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobj/tiny_obj_loader.h>

#include "VKDevice.h"

VKModel::VKModel(const std::string& modelPath)
{
    LoadModel(modelPath);
    CreateVertexBuffer();
    CreateIndexBuffer();
}

VKModel::~VKModel()
{
    vkDestroyBuffer(VK_DEVICE.GetDevice(), vkIndexBuffer, nullptr);
    vkFreeMemory(VK_DEVICE.GetDevice(), vkIndexBufferMemory, nullptr);
    vkDestroyBuffer(VK_DEVICE.GetDevice(), vkVertexBuffer, nullptr);
    vkFreeMemory(VK_DEVICE.GetDevice(), vkVertexBufferMemory, nullptr);
}

void VKModel::Bind(VkCommandBuffer commandBuffer) const
{
    VkBuffer vertexBuffers[] = { vkVertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, vkIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void VKModel::Draw(VkCommandBuffer commandBuffer) const
{
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void VKModel::LoadModel(const std::string& modelPath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str()))
        throw GFX_EXCEPTION(warn + err);

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void VKModel::CreateVertexBuffer()
{
    const VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VK_DEVICE.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(VK_DEVICE.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(VK_DEVICE.GetDevice(), stagingBufferMemory);

    VK_DEVICE.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkVertexBuffer, vkVertexBufferMemory);

    VK_DEVICE.CopyBuffer(stagingBuffer, vkVertexBuffer, bufferSize);

    vkDestroyBuffer(VK_DEVICE.GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VK_DEVICE.GetDevice(), stagingBufferMemory, nullptr);
}

void VKModel::CreateIndexBuffer()
{
    const VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VK_DEVICE.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(VK_DEVICE.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(VK_DEVICE.GetDevice(), stagingBufferMemory);

    VK_DEVICE.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkIndexBuffer, vkIndexBufferMemory);

    VK_DEVICE.CopyBuffer(stagingBuffer, vkIndexBuffer, bufferSize);

    vkDestroyBuffer(VK_DEVICE.GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VK_DEVICE.GetDevice(), stagingBufferMemory, nullptr);
}
