#include "frame.h"

//确定GPU是否合适
bool isDeviceSuitable(VkPhysicalDevice device);

//寻找合适的队列簇
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

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

bool isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device); //需要支持图形指令
	return indices.isComplete();
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0; //获取队列簇
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 &&
			queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i; //设定为找到的队列簇
			break;
		}
		i++;
	}
	return indices;
}
