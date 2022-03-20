#include "frame.h"

void Frame::createSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; //��ʼ״̬ Ĭ��δ�����ź� ��ΪVK_FENCE_CREATE_SIGNALED_BIT��Ĭ�Ϸ����ź�
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
			//vkCreateFence(device, &fenceInfo, nullptr, &imagesInFlight[i]) != VK_SUCCESS)
			//��֪��Ϊʲô �������� ֱ�ӵ�drawFrame:36�ٸ�����inFlightFences�ƺ���bug�Ҳ�����
		{
			throw std::runtime_error("failed to create synchronisation objects for a frame!");
		}
	}
}
