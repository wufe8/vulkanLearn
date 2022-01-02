#ifndef FRAME_H
#define FRAME_H
#pragma once

#define GLFW_INCLUDE_VULKAN //声明glfw使用vulkan api
#include <GLFW/glfw3.h> //glfw用于创建窗口

#include <iostream> //iostream用于控制台输出
#include <stdexcept> //stdexcept支持多种错误类型的处理
#include <functional> //functional用于资源管理
#include <cstdlib> //cstdlib使用EXIT_SUCESS, EXIT_FAILURE宏
#include <vector> //vector数据结构

const int WIDTH = 800; //定义长宽
const int HEIGHT = 600;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation" //隐式开启所有可用校验层
};

#ifdef NDEBUG //debug模式编译将启用检验层
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

//诊断信息输出等级
const VkDebugUtilsMessageSeverityFlagBitsEXT DebugLevel =
VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

//输出vulkan扩展列表
void printVkExtensionList();
//输出可用校验层列表
void printVkLayerList();
//检查可用校验层
bool checkValidationLayerSupport();
/* TODO 输出物理设备
* @param instance vulkan容器 */
void printPhysicalDevices(VkInstance instance);
//获取扩展
std::vector<const char*> getRequiredExtensions();

//队列簇序号存储及其有效性检查
struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	bool isComplete()
	{
		return graphicsFamily >= 0;
	}
};
//查找队列簇
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

//代理函数 获取删除调试层回调管理的函数
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
//校验层调试对象信息设置, 方便vulkan容器设置pNext
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

class Frame //主程序类
{
public:
	GLFWwindow* window = VK_NULL_HANDLE; //窗口定义
	//GLFWwindow* subwindow;
	VkInstance instance = {}; //vulkan容器
	VkDebugUtilsMessengerEXT debugMessenger = {}; //回调函数信息
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; //物理设备
	VkDevice device = VK_NULL_HANDLE; //逻辑设备
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	void run()
	{
		initWindow(); //初始化窗口
		initVulkan(); //初始化Vulkan
		mainLoop(); //渲染事件循环
		cleanup(); //结束前清理资源
	}
	Frame()
	{
		window = nullptr;
	}

private:
	virtual void initWindow()
	{
		glfwInit(); //初始化glfw
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //阻止glfw调用OpenGL(默认情况)
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //禁止调整窗口大小
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //创建窗口 并获取地址
		//subwindow = glfwCreateWindow(WIDTH - 200, HEIGHT - 200, "subwindow", nullptr, nullptr);
	}

	virtual void initVulkan()
	{
		createInstance();
		setupDebugMessenger();
		pickPhysicalDevice();
		createLogicalDevice();
	}

	//创建vulkan容器
	void createInstance();
	//设置校验层及其回调处理
	void setupDebugMessenger();
	//选取物理设备
	void pickPhysicalDevice();
	//创建逻辑设备
	void createLogicalDevice();

	virtual void mainLoop()
	{
		while (!glfwWindowShouldClose(window)) //循环直到点击关闭window窗口
		{
			glfwPollEvents(); //推送事件处理
		}
	}

	virtual void cleanup()
	{
		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}
		vkDestroyDevice(device, nullptr); //删除逻辑设备
		vkDestroyInstance(instance, nullptr); //删除vulkan容器
		glfwDestroyWindow(window); //删除window窗口
		glfwTerminate(); //删除所有剩余资源
	}
};

#endif