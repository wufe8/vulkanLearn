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
*  VK_PRESENT_MODE_MAILBOX_KHR ��FIFO ��ջ��ʱ�ύ��ֱ���滻Ϊ��ͼ�� ��ʵ���������� */

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
	uint32_t imageCount;
	if (presentmode == VK_PRESENT_MODE_MAILBOX_KHR)
	{ //���ػ���������ʹ����Сͼ�����+1��ͼ��
		imageCount = swapChainSupport.capabilities.minImageCount + 1;
	}
	else
	{
		imageCount = swapChainSupport.capabilities.minImageCount;
	}
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
	/* imageUsage ������ʹ�÷�ʽ
	* VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ������ɫ����
	* VK_IMAGE_USAGE_TRANSFER_DST_BIT ���ڴ��� */
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //ָ����β���ͼ��
	/* imageSharingMode ����������ģʽ
	* VK_SHARING_MODE_EXCLUSIVE ��ռģʽ һ��ͼ��ͬһʱ��ֻ�ܱ�һ�����д�ʹ�� ���ܸ���
	* VK_SHARING_MODE_CONCURRENT Эͬģʽ ͼ������ڶ�����дؼ�ʹ�� */
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { uint32_t(indices.graphicsFamily), uint32_t(indices.presentFamily) };
	if (indices.graphicsFamily == indices.presentFamily)
	{ //ͼ���������ֶ�����ͬ ͼ��ʹ�ö�ռģʽ
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}
	else
	{ //ͼ���������ֶ��в�ͬ ͼ��ʹ��Эͬģʽ
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	//ͼ��̶��任����(����ת) ����Ҫ��ֱ��ʹ��currentTransform
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	//����alphaͨ���ķ�ʽ �������������ڽ��л�� ��������ΪVK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	//VK_TRUE��ʾ�����ı����������ڵ����ص���ɫ �������� ������������ؿ��ܱ���
	createInfo.clipped = VK_TRUE;
	//���й����н�����������Ч(��ı䴰�ڴ�С) ��Ҫ�ؽ������� ��ʱ��ʹ�þɽ�����
	createInfo.oldSwapchain = VK_NULL_HANDLE;
	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain!");
	}
	//std::cout << "swap chain created successful." << std::endl;
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}
