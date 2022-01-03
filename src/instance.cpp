#include "frame.h"

void Frame::createInstance() //初始化vulkan
{
	if (enableValidationLayers && !checkValidationLayerSupport()) //使用校验层的情况下提前检查是否存在校验层
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}
	VkApplicationInfo appInfo = {};
	VkInstanceCreateInfo createInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredInstanceExtensions(); //获取并指定vulkan扩展
	createInfo.enabledExtensionCount = int(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
	if (enableValidationLayers) //设置全局校验层
	{
		createInfo.enabledLayerCount = int(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = &debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) //创建实例并检测是否创建成功
	{
		throw std::runtime_error("failed to create instance!");
	}
}

//获取容器扩展
std::vector<const char*> getRequiredInstanceExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); //从glfw处获取与窗口交互的拓展

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); //启用校验层时添加VK_EXT_debug_utils扩展
	}
	return extensions;
}

//检查可用校验层
bool checkValidationLayerSupport()
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
