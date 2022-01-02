#include "frame.h"

/*
* 1. messageSeverity ��Ϣ�ȼ�
*	VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT ���
*	VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT ��ͨ
*	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ����
*	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ����
* 2. messageType ��Ϣ����
*	VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT һ��
*	VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT Υ��
*	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT ����
* 3. pCallbackData ��Ϣ����
*	->pMessage ��null��β����������Ϣ���ַ���
*	->pObjects ��Ϣ��ص�Vulkan����ָ������
*	->objectCount �����Ա��
*/

/*���ܵ�����Ϣ��������ݵĻص�����
* @param messageSeverity ��Ϣ�ȼ�,
* messageType ��Ϣ����,
* pCallbackData ��Ϣ����,
* pUserData ���ûص�����ʱ���ݵ�����ָ��
* @return true: Vulkan API����VK_ERROR_VALIDATION_FAILED_EXT, false: ��������
*/
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	//std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	if (messageSeverity >= DebugLevel)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			std::cerr << "[V] " << pCallbackData->pMessage << std::endl;
			break; //verbose
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			std::cerr << "[I] " << pCallbackData->pMessage << std::endl;
			break; //info
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			std::cerr << "[W] " << pCallbackData->pMessage << std::endl;
			break; //warning
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			std::cerr << "[E] " << pCallbackData->pMessage << std::endl;
			break; //error
		}
	}
	return VK_FALSE;
}

//������ ��ȡ�������Բ�ص�����ĺ���
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

//������ ��ȡɾ�����Բ�ص�����ĺ���
void DestroyDebugUtilsMessengerEXT(VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

//У�����Զ�����Ϣ����, ����vulkan��������pNext
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = //������INFO����Ϣ
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = //��������������Ϣ
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback; //�ص�����ָ��
	createInfo.pUserData = nullptr; //��ѡ �û��Զ������ݵ�ָ��
}

//����У������
void Frame::setupDebugMessenger()
{
	if (!enableValidationLayers) //����Ƿ���Ҫ����У���
	{
		return;
	}
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	populateDebugMessengerCreateInfo(createInfo);
	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug callback!");
	}
	//vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &callback);
	//Ĭ�ϲ����� ��ʹ�ô�������ȡ������ַ
}