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
*  VK_PRESENT_MODE_MAILBOX_KHR 类FIFO 但栈满时提交将直接替换为新图像 可实现三倍缓冲*/

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
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount;
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
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //指定如何操作图像
	
	//TODO: 指定队列簇使用图像的方式
}
