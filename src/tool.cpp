#include "frame.h"

//输出vulkan扩展列表
void printVkExtensionList() 
{
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr); //获取数量
	std::vector<VkExtensionProperties> units(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, units.data()); //获取列表
	std::cout << "available extensions:" << std::endl;
	for (const auto& unit : units) {
		std::cout << "\t" << unit.extensionName << std::endl;
	}
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
}

bool checkValidationLayerSupport() //检查可用校验层
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr); //获取所有可用的校验层
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); //获取列表
	for (const char* layerName : validationLayers) {
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}
		if (!layerFound) {
			return false;
		}
	}
	return true;
}

/* TODO 输出物理设备
* @param instance vulkan容器 */
void printPhysicalDevices(VkInstance instance)
{
}
