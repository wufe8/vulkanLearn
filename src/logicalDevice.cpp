#include "frame.h"

//创建逻辑设备
void Frame::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> queueFamilies = { indices.graphicsFamily, indices.presentFamily };
	const float queuePriorities = 1.0f; //队列优先级 取0.0f至1.0f
	//逐info写入不同队列簇(可能是同一个)
	for (int queueFamily : queueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriorities; //const float*

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
	
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = int(queueCreateInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	
	auto extensions = getRequiredDeviceExtensions();
	createInfo.enabledExtensionCount = int(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (enableValidationLayers) //对设备使用校验层
	{
		createInfo.enabledLayerCount = int(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
}

//获取设备扩展
std::vector<const char*> getRequiredDeviceExtensions()
{
	std::vector<const char*> extensions;
	extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	return extensions;
}
