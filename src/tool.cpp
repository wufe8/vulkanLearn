#include "frame.h"

//输出vulkan扩展列表
void printVkExtensionList() 
{
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr); //获取数量
	std::vector<VkExtensionProperties> units(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, units.data()); //获取列表
	std::cout << "available vulkan extensions:" << std::endl;
	for (const auto& unit : units) {
		std::cout << "\t" << unit.extensionName << std::endl;
	}
	std::cout << std::endl;
}

//输出可用校验层列表
void printVkLayerList()
{
	uint32_t count = 0;
	vkEnumerateInstanceLayerProperties(&count, nullptr); //获取数量
	std::vector<VkLayerProperties> units(count);
	vkEnumerateInstanceLayerProperties(&count, units.data()); //获取列表
	std::cout << "available layer:" << std::endl;
	for (const auto& unit : units) {
		std::cout << "\t" << unit.layerName << std::endl;
	}
	std::cout << std::endl;
}

//输出物理设备 使用单独函数进行传参会导致读取报错 必须为类成员
void Frame::printPhysicalDevices()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(this->instance, &deviceCount, devices.data());
	std::vector<VkPhysicalDeviceProperties> devicesProperties(deviceCount);
	std::vector<VkPhysicalDeviceFeatures> devicesFeatures(deviceCount);
	std::cout << "GPUs info:" << std::endl;
	for (uint32_t i = 0; i < deviceCount; i++) //获取GPUs信息
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

//输出设备扩展列表 使用单独函数进行传参会导致读取报错 必须为类成员
void Frame::printDeviceExtensionList()
{
	uint32_t count = 0;
	vkEnumerateDeviceExtensionProperties(this->physicalDevice, nullptr, &count, nullptr); //获取数量
	std::vector<VkExtensionProperties> units(count);
	vkEnumerateDeviceExtensionProperties(this->physicalDevice, nullptr, &count, units.data()); //获取列表
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
	printPhysicalDevices();
	printDeviceExtensionList();

}
