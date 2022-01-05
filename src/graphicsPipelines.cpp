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

	//��������
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	//�����������ݰ�
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;
	//����������������
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;

	//����װ��
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	/* topology �������ݶ��弸��ͼԪ(��, �߶�, ������)������
	* VK_PRIMITIVE_TOPOLOGY_POINT_LIST
	* ÿ�����㹹��һ����ͼԪ
	* VK_PRIMITIVE_TOPOLOGY_LINE_LIST
	* ÿ�������㹹��һ���߶�ͼԪ
	* VK_PRIMITIVE_TOPOLOGY_LINE_STRIP
	* ͬ�� ������һ���߶�ͼԪ�� ÿ���߶�ͼԪʹ����һ���߶�ͼԪ�ĵڶ���������Ϊ�Լ��ĵ�һ������
	* VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
	* ÿ�������㹹��һ��������ͼԪ
	* VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
	* ͬ�� ������һ��������ͼԪ�� ÿ���߶�ͼԪʹ����һ���߶�ͼԪ�ĵڶ�, ������������Ϊ�Լ��ĵ�һ, �ڶ������� */
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	/* primitiveRestartEnable ��������ͼԪ����
	* ��Ϊ VK_TRUE ʱ ���ʹ�ô��� _STRIP ��β��ͼԪ���� �����ͨ����ɫ����ֵ 0xFFFF ����������ΪͼԪ�ĵ�һ������ */
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	
	//�ӿ� �������������������Ⱦ�����֡��������
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	//������ͼ���С�����봰�ڴ�С��ͬ(�򽻻���ͼ���С���Լ����õ�)
	viewport.width = float(swapChainExtent.width);
	viewport.height = float(swapChainExtent.height);
	//֡��������ֵ��Χ ȡֵ[0.0f, 1.0f]
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	//�ü����� ������һ��������ػ�ʵ�ʴ洢��֡���� �ü����η�Χ������ػᱻ��դ��������
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent; //��������֡�����Ͻ��л��� �򽫷�Χ����Ϊͬ֡���巶Χ

	//�ӿ���ü������������һ��
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	//��դ��
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	/* depthClampEnable VK_TURE ��ƽ���Զƽ�����Ƭ�λᱻ�ض��ڽ�ƽ����Զƽ���� ������ֱ�Ӷ���
	* �����迪����ӦGPU���� */
	rasterizer.depthClampEnable = VK_FALSE;
	//rasterizerDiscardEnable VK_TRUE ����ֹһ��Ƭ�������֡����
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	/* polygonMode ָ��ͼԪ����Ƭ�εķ�ʽ
	* VK_POLYGON_MODE_FILL ����������ڲ�����������ζ�����Ƭ��
	* VK_POLYGON_MODE_LINE ֻ�б߻����Ƭ��
	* VK_POLYGON_MODE_FILL ֻ�ж�������ƫԶ
	* �� VK_POLYGON_MODE_FILL ���ģʽ��Ҫ������ӦGPU���� */
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	/* ָ����ʽ�����߶εĿ�� ���߿���ռ��Ƭ����ĿΪ��λ
	* ���ֵ����Ӳ�� ����1.0f�迪����ӦGPU���� */
	rasterizer.lineWidth = 1.0f;
	//ָ��ʹ�õı����޳����� ���ڽ��ñ����޳�/�޳�����/�޳�����/�޳�˫��
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	//ָ��˳ʱ��Ķ����������� ������ʱ��Ķ�����������
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	//VK_TRUE ���һ������ֵ���߻���Ƭ�������߶ε�б�ʵõ��ı���ֵ�����ֵ�� ����Ӱ��ͼ������
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	/* ���ز���
	* ͨ����϶����ͬ����β���Ƭ�ε���ɫ����������ɫ �Ӷ����ٶ���α�Ե����
	* ����߷ֱ�����Ⱦ����Сͼ�����ʡ���� */
	VkPipelineMultisampleStateCreateInfo mulitisampling = {};
	mulitisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	mulitisampling.sampleShadingEnable = VK_FALSE;
	mulitisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	mulitisampling.minSampleShading = 1.0f;
	mulitisampling.pSampleMask = nullptr;
	mulitisampling.alphaToCoverageEnable = VK_FALSE;
	mulitisampling.alphaToOneEnable = VK_FALSE;

	//��Ȳ��Ժ�ģ�����
	//VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	//depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

	//��ɫ���
	//��ÿ���󶨵�֡������е�������ɫ�������
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	/* �������α����ο�:
	* if (blendEnable)
	* {
	* finalColor.rgb = (srcColorBlendFactor* newColor.rgb) <colorBlendOp> (dstColorBlendFactor* oldColor.rgb);
	* finalColor.a = (srcAlphaBlendFactor* newColor.a) <alphaBlendOp> (dstAlphaBlendFactor* oldColor.a);
	* }
	* else
	* {
	* finalColor = newColor;
	* }
	* finalColor = finalColor & colorWriteMask;
	* ------------------------------------------- *
	* ��:
	* VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	* colorBlendAttachment.colorWriteMask =
	* 	VK_COLOR_COMPONENT_R_BIT |
	* 	VK_COLOR_COMPONENT_G_BIT |
	* 	VK_COLOR_COMPONENT_B_BIT |
	* 	VK_COLOR_COMPONENT_A_BIT;
	* colorBlendAttachment.blendEnable = VK_TRUE;
	* colorBlendAttachment.srcColorBlendFactor = VK_BLEND_SRC_ALPHA;
	* colorBlendAttachment.dstColorBlendFactor = VK_BLEND_ONE_MINUS_SRC_ALPHA;
	* colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	* colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	* colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	* colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	* α����:
	* finalColor.rgb = newAlpha * newColor = (1 - newAlpha) * oldColor;
	* finalColor.a = newAlpha.a;
	*/

	//ȫ����ɫ���
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	/* ����ȫ��λ����
	* ���Զ����þֲ���ɫ���(��ͬȫ����ΪblendEnable = VK_FALSE)
	* �� colorWriteMask ��Ȼ��Ч ���ھ�����Щ��ɫͨ���ܹ�д��֡����
	* ������ʱ Ƭ����ɫ��ֱ�Ӹ���ԭ֡�����д洢����ɫֵ */
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	//��̬״̬ ����ָ����Щ����״̬�����ڲ��ؽ�������½��ж�̬�޸� �����ӿڴ�С �߿�ͻ�ϳ���
	std::vector<VkDynamicState> dynamicStates =
	{
		//VK_DYNAMIC_STATE_VIEWPORT,
		//VK_DYNAMIC_STATE_LINE_WIDTH
	};
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = dynamicStates.size();
	dynamicState.pDynamicStates = dynamicStates.data();
	
	//���߲��ֿ�����ɫ����ʹ��uniform���� ��̬�ؽ����޸� ʵ����ɫ���Ķ�̬����
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

	vkDestroyShaderModule(device, vertShaderModule, nullptr);
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
}