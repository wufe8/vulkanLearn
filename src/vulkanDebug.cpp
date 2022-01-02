#include "frame.h"

/*
* 1. messageSeverity 信息等级
*	VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 诊断
*	VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT 普通
*	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 警告
*	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT 错误
* 2. messageType 信息类型
*	VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 一般
*	VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 违规
*	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT 性能
* 3. pCallbackData 信息内容
*	->pMessage 以null结尾包含调试信息的字符串
*	->pObjects 信息相关的Vulkan对象指针数组
*	->objectCount 数组成员数
*/

/*接受调试信息并输出内容的回调函数
* @param messageSeverity 信息等级,
* messageType 信息类型,
* pCallbackData 信息内容,
* pUserData 设置回调函数时传递的数据指针
* @return true: Vulkan API返回VK_ERROR_VALIDATION_FAILED_EXT, false: 继续运行
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

//代理函数 获取创建调试层回调管理的函数
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

//代理函数 获取删除调试层回调管理的函数
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

//校验层调试对象信息设置, 方便vulkan容器设置pNext
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = //不接收INFO级信息
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = //接收所有类型信息
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback; //回调函数指针
	createInfo.pUserData = nullptr; //可选 用户自定义数据的指针
}

//设置校验层调试
void Frame::setupDebugMessenger()
{
	if (!enableValidationLayers) //检测是否需要开启校验层
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
	//默认不存在 需使用代理函数获取函数地址
}