#include "frame.h"

void Frame::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily; //����ָ����ύ��֧��ͼ�β����Ķ���
	/* ������� ����vulkan������������Ż� ��ֵΪ0��ȫ����ʹ��
	* VK_COMMAND_POOL_CREATE_TRANSIENT_BIT ָ������Ƶ��������¼�µ�ָ��
	* VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT ָ������֮���໥���� ����һ������ */
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
	/* level ָ�������ָ����������Ҫ�����Ǹ�������
	* VK_COMMAND_BUFFER_LEVEL_PRIMARY ���Ա��ύ��������ִ�� �����ܱ�����ָ���������ִ��
	* VK_COMMAND_BUFFER_LEVEL_SECONDARY ����ֱ�ӱ��ύ��������ִ�� �����Ա���Ҫ����������ִ�� */
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
		/* flags ָ�����ô�
		* VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT ���Ǹ�ִ��һ�κ�ͼ�¼��ָ���ָ���
		* VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT ���Ǹ�ֻ��һ����Ⱦ������ʹ�õĸ���ָ���
		* VK_COMMAND_BUFFER_USAGE_SUMULTANEOUS_USE_BIT ���Ǹ���ָ���ȴ�ִ��ʱ��Ȼ�����ύ��ָ����ָ��� (��һ֡��δ������Ⱦ�Ϳ����ύ��һ֡����Ⱦָ��) */
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; //����ָ����޶� ����ָ�������Ҫָ���̳�����״̬
		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		
		renderPassInfo.renderPass = renderPass; //ָ��ʹ�õ���Ⱦ���̶���
		renderPassInfo.framebuffer = swapChainFramebuffers[i]; //ָ��ʹ�õ�֡�������
		//renderArea ָ����Ⱦ������ ��������������ݻᴦ��δ����״̬ һ�㽫������Ϊ�͸��Ŵ�Сһ��
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent; 
		/* ָ��ʹ�� VK_ATTACHMENT_LOAD_OP_CLEAR ��Ǻ���ʹ�õ����ֵ
		* { 0.0f, 0.0f, 0.0f, 1.0f }����͸����ɫ */
		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;
		/* ���пɼ�¼ָ�ָ���ĺ����ĺ�����������vkCmdǰ׺ ����ֵ��Ϊvoid ��˲�����ȫ����ǰ���ý����κδ�����
		* ����:
		* VkCommandBuffer commandBuffer ���ڼ�¼ָ���ָ������
		* const VkRenderPassBeginInfo* pRenderPassBegin ��ʹ�õ���Ⱦ���̵���Ϣ
		* VkSubpassContents contents ��Ⱦ����ṩ����ָ��ı�� ����:
		* VK_SUBPASS_CONTENTS_INLINE ����Ҫִ�е�ָ�����Ҫָ����� û�и���ָ�����Ҫִ��
		* VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS �����Ÿ���ָ����ָ����Ҫִ�� */
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		//��ͼ�ι��� �ڶ�����������ָ�����߶�����ͼ�ι��߻��Ǽ������
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		/* ���Ʋ���
		* ����:
		* VkCommandBuffer commandBuffer ��¼��Ҫִ�е�ָ���ָ������
		* uint32_t vertexCount ������
		* uint32_t instanceCount ������ ����ʵ����Ⱦ ������ʵ����Ⱦ��Ϊ1
		* uint32_t firstVertex ������ɫ������ gl_VertexIndex ��ֵ
		* uint32_t firstInstance ������ɫ������ gl_InstanceIndex ��ֵ
		*/
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}