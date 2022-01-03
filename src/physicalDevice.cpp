#include "frame.h"


const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

//检查设备扩展支持
bool checkDeviceExtensionSupport(VkPhysicalDevice device);

//选取物理设备
void Frame::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}
	std::cout << "[I] find " << deviceCount << " GPUs with Vulkan support." << std::endl;
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
	//已获取GPUs列表
	/*
	std::vector<VkPhysicalDeviceProperties> devicesProperties(deviceCount);
	std::vector<VkPhysicalDeviceFeatures> devicesFeatures(deviceCount);
	for (int i = 0; i < deviceCount; i++) //获取GPUs信息
	{
		vkGetPhysicalDeviceProperties(devices[i], &devicesProperties[i]);
		vkGetPhysicalDeviceFeatures(devices[i], &devicesFeatures[i]);
	}
	*/
	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

bool Frame::isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device); //队列簇支持情况
	bool extensionsSupported = checkDeviceExtensionSupport(device); //设备扩展支持情况
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device); //交换链支持情况
	return indices.isComplete() && extensionsSupported && swapChainSupport.check();
}

QueueFamilyIndices Frame::findQueueFamilies(VkPhysicalDevice device)
{ //因需访问surface 且直接传surface报错 需传整个类 因此添加Frame*参数
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0; //获取队列簇
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0)
		{
			//支持图形指令
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i; //设定为找到的队列簇
			}
			//可呈现图像到窗口表面
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (presentSupport)
			{
				indices.presentFamily = i;
			}
		}
		if (indices.isComplete())
		{
			break;
		}
		i++;
	}
	return indices;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	//获取设备扩展列表 
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
	//创建匹配列表
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName); //匹配成功则删除
	}
	return requiredExtensions.empty(); //如果全部匹配 则全部被删除 返回true
}
