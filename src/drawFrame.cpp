#include "frame.h"

void Frame::drawFrame()
{
	//�ȴ�fence�����ź� ���ĸ�����ָ���ȴ�ȫ��fence(VK_TRUE)������һfence(VK_FALSE)
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;

	/* �ӽ�������ȡһ��ͼ�� (����������չ���� �����֮��صĺ���������KHR��׺)
	* ����:
	* VkDevice device �߼��豸����
	* VkSwapchainKHR swapchain Ҫ��ȡͼ��Ľ�����
	* uint64_t timeout ͼ���ȡ�ĳ�ʱʱ�� ���޷���64λ���͵����ֵ(UINT64_MAX)��ʾû�г�ʱʱ��
	* VkSemaphore semaphore ͼ����ú�֪ͨ���ź�������
	* VkFence fence ͼ����ú�֪ͨ��դ������
	* uint32_t* pImageIndex ������ý�����ͼ������� �������� VkImage����
	* ����ֵ:
	* VK_SUCCESS ���������� ����ʹ��
	* VK_SUBOPTIMAL ��������Ȼ����ʹ�� �����������Ѿ�����׼ȷƥ��
	* VK_ERROR_OUT_OF_DATE_KHR ���������ܼ���ʹ�� ͨ�������ڴ��ڴ�С�ı�� */
	VkResult result = vkAcquireNextImageKHR(
		device,
		swapChain,
		UINT64_MAX,
		imageAvailableSemaphores[currentFrame],
		VK_NULL_HANDLE,
		&imageIndex);
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
	{
		/* �ȴ�������ͼ��ʹ����� ��ֹ��Ⱦ���Ѿ������е�֡
		* ���������Ϊ��ȡ�˴��ڽ�����ͼ��������ͼ���vkAcquireNextImageKHR���򷵻�ͼ���µ� */
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	//��Ǵ�ͼ�����ڱ�ʹ��
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT  ��д����ɫ���ŵĹ��߽׶�
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	//���п�ʼִ��ǰ��Ҫ�ȴ����ź�����������
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	//���п�ʼִ��ǰ��Ҫ�ȴ��Ĺ��߽׶�
	submitInfo.pWaitDstStageMask = waitStages;
	//���ύִ�е�ָ������������
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	//ָ��ָ���ִ�н����󷢳��źŵ��ź�������������
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	vkResetFences(device, 1, &inFlightFences[currentFrame]); //����fenceΪδ�����źŵ�״̬
	/* �ύָ����ͼ��ָ�����
	* �ڶ����������ύ����(֧��ͬʱ�ύ���)
	* ���һ��������ָ���ִ�н�����֪ͨ��դ������ */
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}
	/* ��Ⱦ���̿�ʼ�ͽ���ʱ���Զ�����ͼ�񲼾ֱ任 ���仯�����ڹ��߿�ʼʱ��û�л�ȡ��������ͼ�� �������������������
	* �ӳ����������ü�"renderPass.cpp" */

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	//ָ����ʼ���ֲ�����Ҫ�ȴ����ź���
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain };
	//���ڳ���ͼ��Ľ�����
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	//��Ҫ���ֵ�ͼ���ڽ������е�����
	presentInfo.pImageIndices = &imageIndex;
	//��ȡÿ���������ĳ��ֲ����Ƿ�ɹ�����Ϣ ֻ��һ��������ʱ����ֱ��ʹ�ó��ֺ����ķ���ֵ���ж�
	presentInfo.pResults = nullptr;
	
	//����
	//vkQueuePresentKHR(presentQueue, &presentInfo);
	result = vkQueuePresentKHR(presentQueue, &presentInfo);
	/* ����Ƿ���Ҫ�ؽ�������(�細�ڴ�С�ı�)
	* ע����Ϊͬһʱ��ӽ���������ȡͼ���������� ��������ύ�����к��ټ�� ����return����һ��ѭ�������ظ���ȡͼ��
	* ���Ը��� vkAcquireNextImageKHR �� vkQueuePresentKHR �����ķ���ֵ�жϽ������Ƿ���Ҫ�ؽ� */
	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
	if (result == VK_ERROR_OUT_OF_DATE_KHR || framebufferResized)
	{
		framebufferResized = false;
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	//�ȴ���һ���ύ��ָ�����ִ�� ��ָֹ��ѻ�(�ڴ�й¶) �����˷�GPU��Դ
	//vkQueueWaitIdle(presentQueue);
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT; //��ģ����ʹ���� 0 �� MAX_FRAMES_IN_FLIGHT - 1 ֮��ѭ��
}