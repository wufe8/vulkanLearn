#include "frame.h"

//��ȡ�豸�Ľ�����֧�����
SwapChainSupportDetails Frame::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details = {};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formateCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formateCount, nullptr);
	if (formateCount != 0)
	{
		details.formats.resize(formateCount); //����vector��С
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formateCount, details.formats.data());
	}
	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount); //����vector��С
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}

//ѡ�񽻻�����ʽ
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormates)
{
	//�ж��� ��ѯ�����ʽ�Ƿ����
	for (const auto& availableFormat : availableFormates)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}
	//δ���� ʹ�������趨�ĸ�ʽ
	return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
}

/* VK_PRESENT_MODE_IMMEDIATE_KHR �������䵽��Ļ ���ܵ��»���˺��
*  VK_PRESENT_MODE_FIFO_KHR �Ƚ��ȳ� ջ���ύͼ����ȡͼ����� ���ƴ�ֱͬ��
*  VK_PRESENT_MODE_FIFO_RELAXED_KHR ��FIFO ������һ�δ�ֱ��ɨΪ�� ��ô�´��ύͼ��������ʾ
*  VK_PRESENT_MODE_MAILBOX_KHR ��FIFO ��ջ��ʱ�ύ��ֱ���滻Ϊ��ͼ�� ��ʵ����������*/

//ѡ�����ģʽ
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	//�ж��� ��ѯ�����ʽ�Ƿ����
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			return availablePresentMode;
		}
	}
	//��Ȼ֧��FIFOģʽ ����������ѡ����FIFO
	return VK_PRESENT_MODE_FIFO_KHR;
}

//ѡ�񽻻���Χ(��������ͼ��ķֱ���)
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent; //�����ƴ�������Χʱֱ�ӷ������ֵ
	}
	else
	{
		VkExtent2D actualExtent = { WIDTH, HEIGHT };
		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
		//ȷ���������СֵС ��������ֵ��
		return actualExtent;
	}
}

//����������
void Frame::createSwapChain()
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentmode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{ //��maxImageCountΪ0 ˵��û�����ͼ������
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.presentMode = presentmode;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1; //ָ��ÿ��ͼ������Ĳ�� ͨ��Ϊ1 ��VR��˵�����
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //ָ����β���ͼ��
	
	//TODO: ָ�����д�ʹ��ͼ��ķ�ʽ
}
