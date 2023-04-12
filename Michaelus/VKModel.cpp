#include "VKModel.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobj/tiny_obj_loader.h>

#include "LoggerManager.h"
#include "VKDevice.h"

// VARIABLES STATIQUES
FbxManager* VKModel::pFbxManager = nullptr;

// ********* //

// Construction définissant le modèle et s'il s'agit d'un modèle squeletique
VKModel::VKModel(const std::string& modelPath, bool bIsSkeletal)
	:
	bIsSkeletal(bIsSkeletal)
{
    if (modelPath.find(".obj") != std::string::npos) LoadModelOBJ(modelPath);
    else if (modelPath.find(".fbx") != std::string::npos) LoadModelFBX(modelPath);
    CreateVertexBuffer();
    CreateIndexBuffer();
}

// Destructeur réel pour contrôler la libération de mémoire
void VKModel::Destroy()
{
    for (const auto& vkIndexBuffer : vkIndexBuffers)
    		vkDestroyBuffer(VK_DEVICE.GetDevice(), vkIndexBuffer, nullptr);
    for (const auto& vkIndexBufferMemory : vkIndexBufferMemories)
		vkFreeMemory(VK_DEVICE.GetDevice(), vkIndexBufferMemory, nullptr);
    for (const auto& vkVertexBuffer : vkVertexBuffers)
		vkDestroyBuffer(VK_DEVICE.GetDevice(), vkVertexBuffer, nullptr);
    for (const auto& vkVertexBufferMemory : vkVertexBufferMemories)
		vkFreeMemory(VK_DEVICE.GetDevice(), vkVertexBufferMemory, nullptr);
}

// Lien entre le modèle et le CommandBuffer courant
// On lie les Vertex Buffer et les Index Buffer, c'est à dire les différents sommets du modèle ainsi que l'ordre de rendu de ces sommets
void VKModel::Bind(VkCommandBuffer commandBuffer, size_t shapeIndex) const
{
    if (shapeIndex < vkVertexBuffers.size()) {
        VkBuffer vertexBuffers[] = { vkVertexBuffers.at(shapeIndex) };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, vkIndexBuffers.at(shapeIndex), 0, VK_INDEX_TYPE_UINT32);
    }
}

// Affichage du modèle dans le CommandBuffer courant
void VKModel::Draw(VkCommandBuffer commandBuffer, size_t shapeIndex) const
{
    if (shapeIndex < vkIndexBuffers.size())
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.at(shapeIndex).size()), 1, 0, 0, 0);
}

// Ouverture du Fichier OBJ
void VKModel::LoadModelOBJ(const std::string& modelPath)
{
    // Ouverture du fichier OBJ
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str()))
        throw GFX_EXCEPTION(warn + err);

    // Pour chaque forme présente dans le fichier OBJ
    for (const auto& shape : shapes)
    {
        // On crée un VertexBuffer ...
		std::unordered_map<Vertex, uint32_t> uniqueVertices{};
        std::vector<Vertex> currentShapeVertices;
        std::vector< uint32_t> currentShapeIndices;

        // ... avec toutes les informations définissant le Vertex ...
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};
            // ... Position ...
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };
            // ... Normale ...
            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };
            // ... Coordonnées de texture ...
            vertex.textureCoordinates = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            // ... Couleur
            vertex.color = { 1.0f, 1.0f, 1.0f };

            // Si le vertex n'a pas été parcouru, on enregistre son Index...
            if (!uniqueVertices.contains(vertex))
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(currentShapeVertices.size());
                currentShapeVertices.push_back(vertex);
            }
            // ... pour ensuite lier cet Index dans l'IndexBuffer
            // Cette démarche est très importante, car l'ordre de lecture des Vertex indique si la face est "face à la caméra"
            // ou "dos à la caméra"
        	currentShapeIndices.push_back(uniqueVertices[vertex]);
        }

        vertices.push_back(currentShapeVertices);
        indices.push_back(currentShapeIndices);
    }
}

// Tentative de Lecture d'un fichier FBX
// La documentation de FBX SDK étant très peu détaillée, ou disons très mal expliquée, ce format a été abandonné pour le projet
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

        FbxStringList uvSets;
        fbxMesh->GetUVSetNames(uvSets);
        
        for (int uvIndex = 0; uvIndex < uvSets.GetCount(); uvIndex++) {

			std::unordered_map<Vertex, uint32_t> uniqueVertices{};
            std::vector<Vertex> currentShapeVertices;
            std::vector< uint32_t> currentShapeIndices;

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
                    FbxVector2 uvCoordinates;
                    bool bHasNoUV = false;
                    fbxMesh->GetPolygonVertexUV(j, k, uvSets.GetItemAt(uvIndex)->mString, uvCoordinates, bHasNoUV);
                    if (!bHasNoUV) {
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
                            uniqueVertices[vertex] = static_cast<uint32_t>(currentShapeVertices.size());
                            currentShapeVertices.push_back(vertex);
                        }

                        currentShapeIndices.push_back(uniqueVertices[vertex]);
                    }
                }
            }

            vertices.push_back(currentShapeVertices);
            indices.push_back(currentShapeIndices);
        }
    }

    // ANIMATION

    /*if (bIsSkeletal)
    {
        ProcessSkeletonHierarchy(fbxNode);
        ProcessJointsAndAnimations(fbxNode);
    }
    */
}

void VKModel::ProcessSkeletonHierarchy(FbxNode* inRootNode)
{
    for (int childIndex = 0; childIndex < inRootNode->GetChildCount(); ++childIndex)
    {
        FbxNode* currentNode = inRootNode->GetChild(childIndex);
        ProcessSkeletonHierarchyRecursively(currentNode, 0, 0, -1);
    }
}

void VKModel::ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex)
{
    if (inNode->GetNodeAttribute() && inNode->GetNodeAttribute()->GetAttributeType() && inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
    {
        Joint currJoint;
        currJoint.mParentIndex = inParentIndex;
        currJoint.mName = inNode->GetName();
        skeleton.mJoints.push_back(currJoint);
    }

    for (int i = 0; i < inNode->GetChildCount(); i++)
        ProcessSkeletonHierarchyRecursively(inNode->GetChild(i), inDepth + 1, skeleton.mJoints.size(), myIndex);
}

void VKModel::ProcessJointsAndAnimations(FbxNode* inNode)
{
    FbxMesh* currMesh = inNode->GetMesh();
    unsigned int numOfDeformers = currMesh->GetDeformerCount();

    FbxAMatrix geometryTransform = GetGeometryTransformation(inNode);

    for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
    {
	    
    }
}

FbxAMatrix VKModel::GetGeometryTransformation(FbxNode* inNode)
{
    if (!inNode)
        throw GFX_EXCEPTION("Null for mesh geometry");

    const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
    const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

    return FbxAMatrix(lT, lR, lS);
}

// Création des Vertex Buffers du modèle
void VKModel::CreateVertexBuffer()
{
    vkVertexBuffers.resize(vertices.size());
    vkVertexBufferMemories.resize(vertices.size());

    for (size_t i = 0; i < vertices.size(); i++)
    {
        const VkDeviceSize bufferSize = sizeof(vertices.at(i).at(0)) * vertices.at(i).size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VK_DEVICE.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(VK_DEVICE.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.at(i).data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(VK_DEVICE.GetDevice(), stagingBufferMemory);

        VK_DEVICE.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkVertexBuffers.at(i), vkVertexBufferMemories.at(i));

        VK_DEVICE.CopyBuffer(stagingBuffer, vkVertexBuffers.at(i), bufferSize);

        vkDestroyBuffer(VK_DEVICE.GetDevice(), stagingBuffer, nullptr);
        vkFreeMemory(VK_DEVICE.GetDevice(), stagingBufferMemory, nullptr);
    }
}

// Création des IndexBuffers du modèle
void VKModel::CreateIndexBuffer()
{
    vkIndexBuffers.resize(indices.size());
    vkIndexBufferMemories.resize(indices.size());

    for (size_t i = 0; i < indices.size(); i++)
    {
        const VkDeviceSize bufferSize = sizeof(indices.at(i).at(0)) * indices.at(i).size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VK_DEVICE.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(VK_DEVICE.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.at(i).data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(VK_DEVICE.GetDevice(), stagingBufferMemory);

        VK_DEVICE.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkIndexBuffers.at(i), vkIndexBufferMemories.at(i));

        VK_DEVICE.CopyBuffer(stagingBuffer, vkIndexBuffers.at(i), bufferSize);

        vkDestroyBuffer(VK_DEVICE.GetDevice(), stagingBuffer, nullptr);
        vkFreeMemory(VK_DEVICE.GetDevice(), stagingBufferMemory, nullptr);
    }
}
