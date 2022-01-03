#include "frame.h"

void Frame::createInstance() //��ʼ��vulkan
{
	if (enableValidationLayers && !checkValidationLayerSupport()) //ʹ��У�����������ǰ����Ƿ����У���
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

	auto extensions = getRequiredInstanceExtensions(); //��ȡ��ָ��vulkan��չ
	createInfo.enabledExtensionCount = int(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
	if (enableValidationLayers) //����ȫ��У���
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
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) //����ʵ��������Ƿ񴴽��ɹ�
	{
		throw std::runtime_error("failed to create instance!");
	}
}

//��ȡ������չ
std::vector<const char*> getRequiredInstanceExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); //��glfw����ȡ�봰�ڽ�������չ

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); //����У���ʱ���VK_EXT_debug_utils��չ
	}
	return extensions;
}

//������У���
bool checkValidationLayerSupport()
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
