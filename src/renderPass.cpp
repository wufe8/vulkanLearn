#include "frame.h"

void Frame::createRenderPass()
{
	//代表交换链图像的颜色缓冲附着
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; //采样数 不使用多重采样则设为1
	//指定渲染前后对附着中的数据进行的操作
	/* loadOp 渲染前的颜色和深度缓冲
	* VK_ATTCHMENT_LOAD_OP_LOAD 保持附着的现有内容
	* VK_ATTCHMENT_LOAD_OP_CLEAR 使用一个常量值来清除附着内容
	* VK_ATTCHMENT_LOAD_OP_DONT_CARE 不处理附着的现有内容 */
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	/* storeOp 渲染后的颜色和深度缓冲
	* VK_ATTCHMENT_LOAD_OP_STORE 渲染后内容将被存储以便之后读取
	* VK_ATTCHMENT_LOAD_OP_DONT_CARE 渲染后不读取帧缓冲内容 */
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//loadOp 渲染前的模板缓冲
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//storeOp 渲染后的模板缓冲
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	/* initialLayout/finalLayout 渲染流程开始前/结束后的图像布局方式
	* VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL 图像被用作颜色附着
	* VK_IMAGE_LAYOUT_PRESENT_SRC_KHR 图像被用在交换链中进行呈现操作
	* VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL 图像被用作复制操作的目的图像
	* VK_IMAGE_LAYOUT_UNDEFINED 未定义 不关心 不保证图像内容会被保留 */
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	//子流程附着引用
	VkAttachmentReference colorAttachmentRef = {};
	//引用的附着在附着描述结构体数组中的索引
	colorAttachmentRef.attachment = 0;
	//进行子流程是引用的附着使用的布局方式 vulkan会在子流程开始时自动将引用的附着转换到该图像布局
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	//显式指出这是一个图形渲染的子流程
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef; // 该颜色附着在数组中的索引会被片段着色器使用 对应 layout(location = 0) out vec4 outColor
	subpass.pInputAttachments = nullptr; //被着色器读取的附着
	subpass.pResolveAttachments = nullptr; //用于多重采样的颜色附着
	subpass.pDepthStencilAttachment = nullptr; //用于深度和模板数据的附着
	subpass.pPreserveAttachments = nullptr; //不被该子流程使用 但仍保留数据的附着

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass!");
	}

}