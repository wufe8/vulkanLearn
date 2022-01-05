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

	//顶点输入
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	//顶点数据内容绑定
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;
	//顶点数据属性描述
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;

	//输入装配
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	/* topology 顶点数据定义几何图元(点, 线段, 三角形)的类型
	* VK_PRIMITIVE_TOPOLOGY_POINT_LIST
	* 每个顶点构成一个点图元
	* VK_PRIMITIVE_TOPOLOGY_LINE_LIST
	* 每两个顶点构成一个线段图元
	* VK_PRIMITIVE_TOPOLOGY_LINE_STRIP
	* 同上 但除第一个线段图元外 每个线段图元使用上一个线段图元的第二个顶点作为自己的第一个顶点
	* VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
	* 每三个顶点构成一个三角形图元
	* VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
	* 同上 但除第一个三角形图元外 每个线段图元使用上一个线段图元的第二, 第三个顶点作为自己的第一, 第二个顶点 */
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	/* primitiveRestartEnable 开启重置图元索引
	* 设为 VK_TRUE 时 如果使用带有 _STRIP 结尾的图元类型 则可以通过特色索引值 0xFFFF 将索引重置为图元的第一个顶点 */
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	
	//视口 用于描述被用来输出渲染结果的帧缓冲区域
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	//交换链图像大小可能与窗口大小不同(因交换链图像大小是自己设置的)
	viewport.width = float(swapChainExtent.width);
	viewport.height = float(swapChainExtent.height);
	//帧缓冲的深度值范围 取值[0.0f, 1.0f]
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	//裁剪矩形 定义哪一区域的像素会实际存储于帧缓存 裁剪矩形范围外的像素会被光栅化程序丢弃
	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent; //若在整个帧缓冲上进行绘制 则将范围设置为同帧缓冲范围

	//视口与裁剪矩形需组合在一起
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	//光栅化
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	/* depthClampEnable VK_TURE 近平面和远平面外的片段会被截断在近平面与远平面上 而不是直接丢弃
	* 启用需开启相应GPU特性 */
	rasterizer.depthClampEnable = VK_FALSE;
	//rasterizerDiscardEnable VK_TRUE 将禁止一切片段输出到帧缓冲
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	/* polygonMode 指定图元生成片段的方式
	* VK_POLYGON_MODE_FILL 包括多边形内部的整个多边形都产生片段
	* VK_POLYGON_MODE_LINE 只有边会产生片段
	* VK_POLYGON_MODE_FILL 只有顶点会产生偏远
	* 除 VK_POLYGON_MODE_FILL 外的模式需要启用相应GPU特性 */
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	/* 指定格式化后线段的宽度 以线宽所占的片段数目为单位
	* 最大值依赖硬件 大于1.0f需开启相应GPU特性 */
	rasterizer.lineWidth = 1.0f;
	//指定使用的表面剔除类型 用于禁用表面剔除/剔除背面/剔除正面/剔除双面
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	//指定顺时针的顶点序是正面 还是逆时针的顶点序是正面
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	//VK_TRUE 添加一个常量值或者基于片段所处线段的斜率得到的变量值到深度值上 对阴影贴图很有用
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	/* 多重采样
	* 通过组合多个不同多边形产生片段的颜色决定最终颜色 从而减少多边形边缘走样
	* 比起高分辨率渲染再缩小图像更节省性能 */
	VkPipelineMultisampleStateCreateInfo mulitisampling = {};
	mulitisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	mulitisampling.sampleShadingEnable = VK_FALSE;
	mulitisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	mulitisampling.minSampleShading = 1.0f;
	mulitisampling.pSampleMask = nullptr;
	mulitisampling.alphaToCoverageEnable = VK_FALSE;
	mulitisampling.alphaToOneEnable = VK_FALSE;

	//深度测试和模板测试
	//VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	//depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

	//颜色混合
	//对每个绑定的帧缓冲进行单独的颜色混合配置
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
	/* 具体操作伪代码参考:
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
	* 例:
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
	* 伪代码:
	* finalColor.rgb = newAlpha * newColor = (1 - newAlpha) * oldColor;
	* finalColor.a = newAlpha.a;
	*/

	//全局颜色混合
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	/* 启用全局位运算
	* 将自动禁用局部颜色混合(等同全部设为blendEnable = VK_FALSE)
	* 但 colorWriteMask 仍然有效 用于决定哪些颜色通道能够写入帧缓冲
	* 均禁用时 片段颜色将直接覆盖原帧缓冲中存储的颜色值 */
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	//动态状态 用于指定哪些管线状态可以在不重建的情况下进行动态修改 包括视口大小 线宽和混合常量
	std::vector<VkDynamicState> dynamicStates =
	{
		//VK_DYNAMIC_STATE_VIEWPORT,
		//VK_DYNAMIC_STATE_LINE_WIDTH
	};
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = dynamicStates.size();
	dynamicState.pDynamicStates = dynamicStates.data();
	
	//管线布局可在着色器中使用uniform变量 动态地进行修改 实现着色器的动态配置
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