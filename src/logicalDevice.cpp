#include "frame.h"

//创建逻辑设备
void Frame::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
	queueCreateInfo.queueCount = 1;
	const float queuePriorities = 1.0f; //队列优先级 取0.0f至1.0f
	queueCreateInfo.pQueuePriorities = &queuePriorities; //const float*

	VkPhysicalDeviceFeatures deviceFeatures = {};
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
	
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = queueCreateInfo.queueCount;
	createInfo.pEnabledFeatures = &deviceFeatures;
	
	createInfo.enabledExtensionCount = 0;
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
}
