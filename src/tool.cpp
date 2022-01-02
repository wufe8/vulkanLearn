#include "frame.h"

//���vulkan��չ�б�
void printVkExtensionList() 
{
	uint32_t count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr); //��ȡ����
	std::vector<VkExtensionProperties> units(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, units.data()); //��ȡ�б�
	std::cout << "available extensions:" << std::endl;
	for (const auto& unit : units) {
		std::cout << "\t" << unit.extensionName << std::endl;
	}
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
}

bool checkValidationLayerSupport() //������У���
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr); //��ȡ���п��õ�У���
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); //��ȡ�б�
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

/* TODO ��������豸
* @param instance vulkan���� */
void printPhysicalDevices(VkInstance instance)
{
}
