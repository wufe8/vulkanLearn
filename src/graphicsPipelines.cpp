#include "frame.h"

void Frame::createGraphicsPipelines()
{
	//读取顶点着色器SPIR-V字节码
	auto vertShaderCode = readFileBinary("shaders/vert.spv");
	//读取片段着色器SPIR-V字节码
	auto fragShaderCode = readFileBinary("shaders/frag.spv");
	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	//指定调用的着色器函数 用于在多个片段着色器中选择需要的
	vertShaderStageInfo.pName = "main";
	//指定着色器用到的常量
	vertShaderStageInfo.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	//指定调用的着色器函数 用于在多个片段着色器中选择需要的
	fragShaderStageInfo.pName = "main";
	//指定着色器用到的常量
	fragShaderStageInfo.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo shaderStages[] = {
		vertShaderStageInfo, fragShaderStageInfo
	};

	vkDestroyShaderModule(device, vertShaderModule, nullptr);
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
}