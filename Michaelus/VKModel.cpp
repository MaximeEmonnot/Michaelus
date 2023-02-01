#include "VKModel.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobj/tiny_obj_loader.h>

#include "VKDevice.h"

FbxManager* VKModel::pFbxManager = nullptr;

VKModel::VKModel(const std::string& modelPath)
{
    if (modelPath.find(".obj") != std::string::npos) LoadModelOBJ(modelPath);
    else if (modelPath.find(".fbx") != std::string::npos) LoadModelFBX(modelPath);
    CreateVertexBuffer();
    CreateIndexBuffer();
}

VKModel::~VKModel()
{
    OutputDebugStringA("Model destroyed!\n");
}

void VKModel::Destroy()
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

void VKModel::LoadModelOBJ(const std::string& modelPath)
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

            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            vertex.textureCoordinates = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            if (!uniqueVertices.contains(vertex))
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void VKModel::LoadModelFBX(const std::string& modelPath)
{
    if (!pFbxManager)
    {
        pFbxManager = FbxManager::Create();
        FbxIOSettings* ioSettings = FbxIOSettings::Create(pFbxManager, IOSROOT);
        pFbxManager->SetIOSettings(ioSettings);
    }

    FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "");
    FbxScene* fbxScene = FbxScene::Create(pFbxManager, "");

    if (!fbxImporter->Initialize(modelPath.c_str(), -1, pFbxManager->GetIOSettings()))
        throw GFX_EXCEPTION("Failed to initialize FBX File !");

    if (!fbxImporter->Import(fbxScene))
        throw GFX_EXCEPTION("Failed to import FBX File !");

    fbxImporter->Destroy();

    FbxNode* fbxNode = fbxScene->GetRootNode();

    for (int i = 0; i< fbxNode->GetChildCount(); i++)
    {
        FbxNode* fbxChildNode = fbxNode->GetChild(i);

        if (!fbxChildNode->GetNodeAttribute()) continue;

        FbxNodeAttribute::EType attributeType = fbxChildNode->GetNodeAttribute()->GetAttributeType();

        if (attributeType != FbxNodeAttribute::eMesh) continue;

        FbxMesh* fbxMesh = (FbxMesh*)fbxChildNode->GetNodeAttribute();

        FbxVector4* fbxVertices = fbxMesh->GetControlPoints();

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (int j = 0; j < fbxMesh->GetPolygonVertexCount(); j++)
        {
            const int polygonVertices = fbxMesh->GetPolygonSize(j);

            for (int k = 0; k < polygonVertices; k++)
            {
                // Position
                const int controlPointIndex = fbxMesh->GetPolygonVertex(j, k);
                // Normal
                FbxVector4 fbxNormals;
                fbxMesh->GetPolygonVertexNormal(j, k, fbxNormals);
                // Texture Coords
                FbxStringList uvSets;
                fbxMesh->GetUVSetNames(uvSets);
                FbxVector2 uvCoordinates;
                bool hasUV = false;
                fbxMesh->GetPolygonVertexUV(j, k, uvSets.GetItemAt(0)->mString, uvCoordinates, hasUV);

                Vertex vertex{};

                vertex.position = {
                    static_cast<float>(fbxVertices[controlPointIndex].mData[0]),
                    static_cast<float>(fbxVertices[controlPointIndex].mData[1]),
                    static_cast<float>(fbxVertices[controlPointIndex].mData[2])
                };

                vertex.normal = {
                        static_cast<float>(fbxNormals.mData[0]),
                        static_cast<float>(fbxNormals.mData[1]),
                        static_cast<float>(fbxNormals.mData[2])
                };

                vertex.textureCoordinates = {
                    static_cast<float>(uvCoordinates.mData[0]),
                    1.f - static_cast<float>(uvCoordinates.mData[1])
                };

                vertex.color = { 1.0f, 1.0f, 1.0f };

                if (!uniqueVertices.contains(vertex))
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
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
