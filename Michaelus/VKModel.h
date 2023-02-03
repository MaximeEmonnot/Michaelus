#pragma once
#include "Vulkan.h"

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <fbxsdk.h>

struct Vertex
{
	bool operator==(const Vertex& other) const {
		return position == other.position && color == other.color && textureCoordinates == other.textureCoordinates;
	}

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescription()
	{
		std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, normal);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, color);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(Vertex, textureCoordinates);

		return attributeDescriptions;
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 textureCoordinates;
	glm::vec4 boneIDs;
	glm::vec4 weights;
};

struct AnimKeyFrame
{
	AnimKeyFrame() : mNext(nullptr) {}

	FbxLongLong mFrameNum;
	FbxAMatrix mGlobalTransform;
	AnimKeyFrame* mNext;
};

struct Joint
{
	int mParentIndex;
	std::string mName;
	FbxAMatrix mGlobalBindposeInverse;
	AnimKeyFrame* mAnimation;
	FbxNode* mNode;

	Joint() :
		mNode(nullptr),
		mAnimation(nullptr)
	{
		mGlobalBindposeInverse.SetIdentity();
		mParentIndex = -1;
	}

	~Joint()
	{
		while (mAnimation)
		{
			AnimKeyFrame* temp = mAnimation->mNext;
			delete mAnimation;
			mAnimation = temp;
		}
	}
};

struct Skeleton
{
	std::vector<Joint> mJoints;
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.textureCoordinates) << 1);
		}
	};
}


class VKModel
{
public:
	VKModel(const std::string& modelPath, bool bIsSkeletal = false);
	~VKModel();

	void Destroy();

	void Bind(VkCommandBuffer commandBuffer, size_t shapeIndex) const;
	void Draw(VkCommandBuffer commandBuffer, size_t shapeIndex) const;

private:
	void LoadModelOBJ(const std::string& modelPath);

	void LoadModelFBX(const std::string& modelPath);
	void ProcessSkeletonHierarchy(FbxNode* inRootNode);
	void ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);
	void ProcessJointsAndAnimations(FbxNode* inNode);
	FbxAMatrix GetGeometryTransformation(FbxNode* inNode);

	void CreateVertexBuffer();
	void CreateIndexBuffer();

private:
	static FbxManager* pFbxManager;

	bool bIsSkeletal = false;
	Skeleton skeleton;

	std::vector<std::vector<Vertex>> vertices;
	std::vector<std::vector<uint32_t>> indices;

	std::vector<VkBuffer> vkVertexBuffers;
	std::vector<VkDeviceMemory> vkVertexBufferMemories;
	std::vector<VkBuffer> vkIndexBuffers;
	std::vector<VkDeviceMemory> vkIndexBufferMemories;
};

