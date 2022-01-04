#include "frame.h"

void Frame::createGraphicsPipelines()
{
	//��ȡ������ɫ��SPIR-V�ֽ���
	auto vertShaderCode = readFileBinary("shaders/vert.spv");
	//��ȡƬ����ɫ��SPIR-V�ֽ���
	auto fragShaderCode = readFileBinary("shaders/frag.spv");
	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	//ָ�����õ���ɫ������ �����ڶ��Ƭ����ɫ����ѡ����Ҫ��
	vertShaderStageInfo.pName = "main";
	//ָ����ɫ���õ��ĳ���
	vertShaderStageInfo.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	//ָ�����õ���ɫ������ �����ڶ��Ƭ����ɫ����ѡ����Ҫ��
	fragShaderStageInfo.pName = "main";
	//ָ����ɫ���õ��ĳ���
	fragShaderStageInfo.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo shaderStages[] = {
		vertShaderStageInfo, fragShaderStageInfo
	};

	vkDestroyShaderModule(device, vertShaderModule, nullptr);
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
}