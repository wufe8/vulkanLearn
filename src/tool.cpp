#include "frame.h"

//���vulkan��չ�б�
void printVkExtensionList() 
{
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr); //��ȡ����
	std::vector<VkExtensionProperties> units(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, units.data()); //��ȡ�б�
	std::cout << "available vulkan extensions:" << std::endl;
	for (const auto& unit : units) {
		std::cout << "\t" << unit.extensionName << std::endl;
	}
	std::cout << std::endl;
}

//�������У����б�
void printVkLayerList()
{
	uint32_t count = 0;
	vkEnumerateInstanceLayerProperties(&count, nullptr); //��ȡ����
	std::vector<VkLayerProperties> units(count);
	vkEnumerateInstanceLayerProperties(&count, units.data()); //��ȡ�б�
	std::cout << "available layer:" << std::endl;
	for (const auto& unit : units) {
		std::cout << "\t" << unit.layerName << std::endl;
	}
	std::cout << std::endl;
}

/* TODO ��������豸
* @param instance vulkan���� */
void printPhysicalDevices(Frame* pFrame)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(pFrame->instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(pFrame->instance, &deviceCount, devices.data());
	std::vector<VkPhysicalDeviceProperties> devicesProperties(deviceCount);
	std::vector<VkPhysicalDeviceFeatures> devicesFeatures(deviceCount);
	std::cout << "GPUs info:" << std::endl;
	for (uint32_t i = 0; i < deviceCount; i++) //��ȡGPUs��Ϣ
	{
		vkGetPhysicalDeviceProperties(devices[i], &devicesProperties[i]);
		vkGetPhysicalDeviceFeatures(devices[i], &devicesFeatures[i]);
		std::cout << "\tGPU " << i << std::endl;
		std::cout << "\tapiVersion: " << devicesProperties[i].apiVersion << std::endl;
		std::cout << "\tdriverVersion: " << devicesProperties[i].driverVersion << std::endl;
		std::cout << "\tdeviceID: " << devicesProperties[i].deviceID << std::endl;
		std::cout << "\tdeviceName: " << devicesProperties[i].deviceName << std::endl;
	}
	std::cout << std::endl;
}

//����豸��չ�б�
void printDeviceExtensionList(Frame* pFrame)
{
	uint32_t count = 0;
	vkEnumerateDeviceExtensionProperties(pFrame->physicalDevice, nullptr, &count, nullptr); //��ȡ����
	std::vector<VkExtensionProperties> units(count);
	vkEnumerateDeviceExtensionProperties(pFrame->physicalDevice, nullptr, &count, units.data()); //��ȡ�б�
	std::cout << "available device extensions:" << std::endl;
	for (const auto& unit : units) {
		std::cout << "\t" << unit.extensionName << std::endl;
	}
	std::cout << std::endl;
}

void Frame::printDebug()
{
	printVkExtensionList();
	printVkLayerList();
	printPhysicalDevices(this);
	printDeviceExtensionList(this);

}
