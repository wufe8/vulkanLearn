#include "frame.h"

//获取设备的交换链支持情况
SwapChainSupportDetails Frame::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details = {};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formateCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formateCount, nullptr);
	if (formateCount != 0)
	{
		details.formats.resize(formateCount); //调整vector大小
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formateCount, details.formats.data());
	}
	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount); //调整vector大小
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}

//选择交换链格式
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormates)
{
	//有定义 查询需求格式是否存在
	for (const auto& availableFormat : availableFormates)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}
	//未定义 使用自行设定的格式
	return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
}

/* VK_PRESENT_MODE_IMMEDIATE_KHR 立即传输到屏幕 可能导致画面撕裂
*  VK_PRESENT_MODE_FIFO_KHR 先进先出 栈顶提交图像与取图像输出 类似垂直同步
*  VK_PRESENT_MODE_FIFO_RELAXED_KHR 类FIFO 但若上一次垂直回扫为空 那么下次提交图像将立即显示
*  VK_PRESENT_MODE_MAILBOX_KHR 类FIFO 但栈满时提交将直接替换为新图像 可实现三倍缓冲 */

//选择呈现模式
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	//有定义 查询需求格式是否存在
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			return availablePresentMode;
		}
	}
	//必然支持FIFO模式 若无其他可选则用FIFO
	return VK_PRESENT_MODE_FIFO_KHR;
}

//选择交换范围(交换链中图像的分辨率)
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent; //无限制窗口允许范围时直接返回最大值
	}
	else
	{
		VkExtent2D actualExtent = { WIDTH, HEIGHT };
		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
		//确保不会比最小值小 不会比最大值大
		return actualExtent;
	}
}

//创建交换链
void Frame::createSwapChain()
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentmode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
	uint32_t imageCount;
	if (presentmode == VK_PRESENT_MODE_MAILBOX_KHR)
	{ //三重缓冲至少需使用最小图像个数+1的图像
		imageCount = swapChainSupport.capabilities.minImageCount + 1;
	}
	else
	{
		imageCount = swapChainSupport.capabilities.minImageCount;
	}
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{ //若maxImageCount为0 说明没有最大图像限制
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
	createInfo.imageArrayLayers = 1; //指定每个图像包含的层次 通常为1 对VR来说会更多
	/* imageUsage 交换链使用方式
	* VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT 用于颜色附着
	* VK_IMAGE_USAGE_TRANSFER_DST_BIT 用于传输 */
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //指定如何操作图像
	/* imageSharingMode 交换链访问模式
	* VK_SHARING_MODE_EXCLUSIVE 独占模式 一张图像同一时间只能被一个队列簇使用 性能更好
	* VK_SHARING_MODE_CONCURRENT 协同模式 图像可以在多个队列簇间使用 */
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { uint32_t(indices.graphicsFamily), uint32_t(indices.presentFamily) };
	if (indices.graphicsFamily == indices.presentFamily)
	{ //图像队列与呈现队列相同 图像使用独占模式
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}
	else
	{ //图像队列与呈现队列不同 图像使用协同模式
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	//图像固定变换操作(如旋转) 不需要则直接使用currentTransform
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	//处理alpha通道的方式 用于与其他窗口进行混合 忽略着设为VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	//VK_TRUE表示不关心被其他窗口遮挡像素的颜色 提升性能 但会读窗口像素可能报错
	createInfo.clipped = VK_TRUE;
	//运行过程中交换链可能生效(如改变窗口大小) 需要重建交换链 此时需使用旧交换链
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
