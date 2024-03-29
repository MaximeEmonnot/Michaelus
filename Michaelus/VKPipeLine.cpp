#include "VKPipeLine.h"

#include <fstream>

#include "VKDevice.h"
#include "VKModel.h"
#include "VKSwapChain.h"
#include "VKDescriptor.h"

// Constructeur d�finissant le Vertex Shader, le Fragment Shader, le lien avec le SwapChain et le Descriptor associ� aux Shaders
VKPipeLine::VKPipeLine(const std::string& vertexFilePath, const std::string& fragmentFilePath, const VKSwapChain& swapChain, VKDescriptor& descriptor)
{
	CreateGraphicsPipeline(vertexFilePath, fragmentFilePath, swapChain, descriptor);
}

// Destructeur r�el pour contr�ler la lib�ration de m�moire
void VKPipeLine::Destroy()
{
    vkDestroyPipeline(VK_DEVICE.GetDevice(), vkGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(VK_DEVICE.GetDevice(), vkPipelineLayout, nullptr);
}

// Lien du Pipeline avec le CommandBuffer courant
void VKPipeLine::Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame, VKDescriptor& descriptor) const
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &descriptor.GetDescriptorSets().at(currentFrame), 0, nullptr);
}

// Cr�ation du Pipeline Graphique
// Ce Pipeline est constitu� de plusieurs �tapes qui seront d�crites tout au long de la m�thode
void VKPipeLine::CreateGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentFilePath, const VKSwapChain& swapChain, VKDescriptor& descriptor)
{
    // Lecture des Vertex et Fragment Shaders
    auto vertexShaderCode = ReadShaderFile(vertexFilePath);
    auto fragmentShaderCode = ReadShaderFile(fragmentFilePath);

    // Cr�ation des Modules pour les Shaders
    VkShaderModule vertexShaderModule = CreateShaderModule(vertexShaderCode);
    VkShaderModule fragmentShaderModule = CreateShaderModule(fragmentShaderCode);

    // Informations pour le Vertex Shader
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = vertexShaderModule;
    vertexShaderStageInfo.pName = "main";

    // Informations pour Fragment Shader
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";

    // Diff�rentes �tapes du Pipeline (Shaders)
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };

    auto bindingDescription = Vertex::GetBindingDescription();
    auto attributeDescriptions = Vertex::GetAttributeDescription();

    // Param�tres pour les vertex
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // Param�tres pour les entr�es
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // D�finition du viewport, c'est-�-dire la fen�tre de rendu
    VkViewport viewport{};
    viewport.x = 0.f;
    viewport.y = 0.f;
    viewport.width = static_cast<float>(swapChain.GetWidth());
    viewport.height = static_cast<float>(swapChain.GetHeight());
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;

    // Scissor, pour limiter la fen�tre de rendu dans le champ du SwapChain
    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapChain.GetSwapChainExtent();

    // D�finition des �tats dynamiques du Pipeline
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    // D�finition des �tats du Viewport 
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // D�finition du Depth Stencil
    // Ce dernier permet le calcul de la profondeur entre plusieurs faces.
    // C'est ce qui permet d'afficher des objets sur plusieurs plans. Sans ce Depth Stencil, tout serait rendu au m�me plan
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.f;
    depthStencil.maxDepthBounds = 1.f;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {};
    depthStencil.back = {};

    // D�finition du Rasterizer
    // Ce dernier permet de remplir les triangle d�finis par les Vertex
    // C'est ce qui permet entre autre de d�finir si une face est dans le bon sens ou non, et d'afficher seulement les faces qui "regardent vers la cam�ra"
    // De m�me, c'est ce qui permet de d�finir quels pixels colori�s pour remplir le triangle
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.f;
    rasterizer.depthBiasClamp = 0.f;
    rasterizer.depthBiasSlopeFactor = 0.f;

    // D�finition du Multisampling
    // Grossi�rement, il s'agit de subdiviser les pixels pour augmenter la d�finition de l'image
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.rasterizationSamples = VK_DEVICE.GetMSAASamples();
    multisampling.minSampleShading = 0.2f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    // D�finition du ColorBlending
    // Cela permet de g�rer tout ce qui est transparance, d�fini par la valeur Alpha d'une couleur d'une face
    // Ainsi, une face ayant pour alpha 0.5 aura une transparance de 50%
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.f;
    colorBlending.blendConstants[1] = 0.f;
    colorBlending.blendConstants[2] = 0.f;
    colorBlending.blendConstants[3] = 0.f;

    // D�finition des informations du Layout du Pipeline
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = descriptor.GetDescriptorSetLayout();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (VK_FAILED(vkCreatePipelineLayout(VK_DEVICE.GetDevice(), &pipelineLayoutInfo, nullptr, &vkPipelineLayout)))
        throw GFX_EXCEPTION("Failed to create pipeline layout!");

    // D�finition finale du Pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = vkPipelineLayout;
    pipelineInfo.renderPass = swapChain.GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    // Cr�ation du Pipeline
    if (VK_FAILED(vkCreateGraphicsPipelines(VK_DEVICE.GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vkGraphicsPipeline)))
        throw GFX_EXCEPTION("Failed to create Graphics Pipeline!");

    // Destruction des Modules de Shader
    vkDestroyShaderModule(VK_DEVICE.GetDevice(), fragmentShaderModule, nullptr);
    vkDestroyShaderModule(VK_DEVICE.GetDevice(), vertexShaderModule, nullptr);
}

// Lecture du Shader
std::vector<char> VKPipeLine::ReadShaderFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) throw GFX_EXCEPTION("Failed to open file!");

	const size_t fileSize = file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

// Cr�ation du Module de Shader
VkShaderModule VKPipeLine::CreateShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};

	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (VK_FAILED(vkCreateShaderModule(VK_DEVICE.GetDevice(), &createInfo, nullptr, &shaderModule)))
		throw std::runtime_error("Failed to create shader module!");

	return shaderModule;
}
