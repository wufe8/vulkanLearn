#include "frame.h"


const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

//����豸��չ֧��
bool checkDeviceExtensionSupport(VkPhysicalDevice device);

//ѡȡ�����豸
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
	//�ѻ�ȡGPUs�б�
	/*
	std::vector<VkPhysicalDeviceProperties> devicesProperties(deviceCount);
	std::vector<VkPhysicalDeviceFeatures> devicesFeatures(deviceCount);
	for (int i = 0; i < deviceCount; i++) //��ȡGPUs��Ϣ
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
	QueueFamilyIndices indices = findQueueFamilies(device); //���д�֧�����
	bool extensionsSupported = checkDeviceExtensionSupport(device); //�豸��չ֧�����
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device); //������֧�����
	return indices.isComplete() && extensionsSupported && swapChainSupport.check();
}

QueueFamilyIndices Frame::findQueueFamilies(VkPhysicalDevice device)
{ //�������surface ��ֱ�Ӵ�surface���� �贫������ ������Frame*����
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0; //��ȡ���д�
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0)
		{
			//֧��ͼ��ָ��
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i; //�趨Ϊ�ҵ��Ķ��д�
			}
			//�ɳ���ͼ�񵽴��ڱ���
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
	//��ȡ�豸��չ�б� 
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
	//����ƥ���б�
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName); //ƥ��ɹ���ɾ��
	}
	return requiredExtensions.empty(); //���ȫ��ƥ�� ��ȫ����ɾ�� ����true
}
