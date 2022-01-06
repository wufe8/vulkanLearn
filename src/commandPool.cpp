#include "frame.h"

void Frame::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily; //绘制指令将被提交给支持图形操作的队列
	/* 创建标记 方便vulkan驱动程序进行优化 赋值为0则全部不使用
	* VK_COMMAND_POOL_CREATE_TRANSIENT_BIT 指令缓冲对象被频繁用来记录新的指令
	* VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT 指令缓冲对象之间相互独立 不会一起重置 */
	poolInfo.flags = 0;
	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void Frame::createCommandBuffers()
{
	commandBuffers.resize(swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	/* level 指定分配的指令缓冲对象是主要对象还是辅助对象
	* VK_COMMAND_BUFFER_LEVEL_PRIMARY 可以被提交到队列上执行 但不能被其他指令缓冲对象调用执行
	* VK_COMMAND_BUFFER_LEVEL_SECONDARY 不能直接被提交到队列上执行 但可以被主要缓冲对象调用执行 */
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = uint32_t(commandBuffers.size());
	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command buffers!");
	}
	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		/* flags 指令缓冲的用处
		* VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT 这是个执行一次后就记录新指令的指令缓冲
		* VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT 这是个只在一个渲染流程内使用的辅助指令缓冲
		* VK_COMMAND_BUFFER_USAGE_SUMULTANEOUS_USE_BIT 这是个在指令缓冲等待执行时仍然可以提交该指令缓冲的指令缓冲 (上一帧还未结束渲染就可以提交下一帧的渲染指令) */
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; //辅助指令缓冲限定 用于指定其从主要指令缓冲继承来的状态
		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		
		renderPassInfo.renderPass = renderPass; //指定使用的渲染流程对象
		renderPassInfo.framebuffer = swapChainFramebuffers[i]; //指定使用的帧缓冲对象
		//renderArea 指定渲染的区域 区域外的像素数据会处于未定义状态 一般将其设置为和附着大小一样
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent; 
		/* 指定使用 VK_ATTACHMENT_LOAD_OP_CLEAR 标记后所使用的清除值
		* { 0.0f, 0.0f, 0.0f, 1.0f }即不透明黑色 */
		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;
		/* 所有可记录指令到指令缓冲的函数的函数名都带有vkCmd前缀 返回值均为void 因此操作完全结束前不用进行任何错误处理
		* 参数:
		* VkCommandBuffer commandBuffer 用于记录指令的指令缓冲对象
		* const VkRenderPassBeginInfo* pRenderPassBegin 所使用的渲染流程的信息
		* VkSubpassContents contents 渲染如何提供绘制指令的标记 包含:
		* VK_SUBPASS_CONTENTS_INLINE 所有要执行的指令都在主要指令缓冲中 没有辅助指令缓冲需要执行
		* VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS 有来着辅助指令缓冲的指令需要执行 */
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		//绑定图形管线 第二个参数用于指定管线对象是图形管线还是计算管线
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		/* 绘制操作
		* 参数:
		* VkCommandBuffer commandBuffer 记录有要执行的指令的指令缓冲对象
		* uint32_t vertexCount 顶点数
		* uint32_t instanceCount 容器数 用于实例渲染 不进行实例渲染则为1
		* uint32_t firstVertex 定义着色器变量 gl_VertexIndex 的值
		* uint32_t firstInstance 定义着色器变量 gl_InstanceIndex 的值
		*/
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}