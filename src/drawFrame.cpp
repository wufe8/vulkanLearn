#include "frame.h"

void Frame::drawFrame()
{
	uint32_t imageIndex;
	/* �ӽ�������ȡһ��ͼ�� (����������չ���� �����֮��صĺ���������KHR��׺)
	* ����:
	* VkDevice device �߼��豸����
    * VkSwapchainKHR swapchain Ҫ��ȡͼ��Ľ�����
    * uint64_t timeout ͼ���ȡ�ĳ�ʱʱ�� ���޷���64λ���͵����ֵ��ʾû�г�ʱʱ��
    * VkSemaphore semaphore ͼ����ú�֪ͨ���ź�������
    * VkFence fence ͼ����ú�֪ͨ��դ������
    * uint32_t* pImageIndex ������ý�����ͼ������� �������� VkImage����

	* */
	vkAcquireNextImageKHR(
		device,
		swapChain,
		std::numeric_limits<uint64_t>::max(),
		imageAvailableSemaphore,
		VK_NULL_HANDLE,
		&imageIndex);
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT  ��д����ɫ���ŵĹ��߽׶�
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	//���п�ʼִ��ǰ��Ҫ�ȴ����ź�����������
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	//���п�ʼִ��ǰ��Ҫ�ȴ��Ĺ��߽׶�
	submitInfo.pWaitDstStageMask = waitStages;
	//���ύִ�е�ָ������������
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	//ָ��ָ���ִ�н����󷢳��źŵ��ź�������������
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	/* �ύָ����ͼ��ָ����� �ڶ����������ύ����(֧��ͬʱ�ύ���) ���һ��������ָ���ִ�н�����֪ͨ��դ������
	*/
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
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
	vkQueuePresentKHR(presentQueue, &presentInfo);
	//�ȴ���һ���ύ��ָ�����ִ�� ��ָֹ��ѻ�(�ڴ�й¶) �����˷�GPU��Դ
	vkQueueWaitIdle(presentQueue);
}