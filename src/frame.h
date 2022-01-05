#ifndef FRAME_H
#define FRAME_H
#pragma once

//#define VK_USE_PLATFORM_WIN32_KHR //使用windows平台原生功能
#define GLFW_INCLUDE_VULKAN //声明glfw使用vulkan api
#include <GLFW/glfw3.h> //glfw用于创建窗口
//#define GLFW_EXPOSE_NATIVE_WIN32 //使用windows平台原生功能
//#include <GLFW/glfw3native.h> //使用平台原生

#include <iostream> //iostream用于控制台输出
#include <stdexcept> //stdexcept支持多种错误类型的处理
#include <functional> //functional用于资源管理
#include <cstdlib> //cstdlib使用EXIT_SUCESS, EXIT_FAILURE宏
#include <vector> //vector数据结构
#include <set> //set数据结构
#include <algorithm> //数学运算比较
#include <fstream> //文件处理

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

class Frame;

//输出vulkan扩展列表
void printVkExtensionList();
//输出可用校验层列表
void printVkLayerList();
/* TODO 输出物理设备
* @param instance vulkan容器 */
void printPhysicalDevices(Frame* pFrame);
//输出设备扩展列表
void printDeviceExtensionList(Frame* pFrame);
//读取二进制文件
static std::vector<char> readFileBinary(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}
	//因为从尾部开始读取 因此可由读取位置得知文件大小 方便分配数组空间
	size_t fileSize = size_t(file.tellg());
	std::vector<char> buffer(fileSize);
	file.seekg(0); //跳到文件头
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

//检查可用校验层
bool checkValidationLayerSupport();
//检查交换链支持情况
//bool querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);


//获取容器扩展
std::vector<const char*> getRequiredInstanceExtensions();
//获取设备扩展
std::vector<const char*> getRequiredDeviceExtensions();

//队列簇序号存储及其有效性检查
struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily  = -1;
	bool isComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

/*交换链支持细节
* @manber
*/
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
	bool check()
	{
		return !formats.empty() && !presentModes.empty();
	}
};

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
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; //物理设备 无需清除
	VkDevice device = VK_NULL_HANDLE; //逻辑设备
	VkQueue graphicsQueue = VK_NULL_HANDLE; //图像队列 清除逻辑设备时自动清除
	VkQueue presentQueue = VK_NULL_HANDLE; //呈现队列 清除逻辑设备时自动清除
	VkSurfaceKHR surface = VK_NULL_HANDLE; //窗口表面抽象
	VkSwapchainKHR swapChain = VK_NULL_HANDLE; //交换链
	std::vector<VkImage> swapChainImages = {}; //交换链图像 清除交换链时自动清除
	VkFormat swapChainImageFormat = {}; //交换链图像格式
	VkExtent2D swapChainExtent = {}; //交换链图像分辨率
	std::vector<VkImageView> swapChainImageViews = {}; //图像视图
	VkRenderPass renderPass = {}; //渲染流程
	VkPipelineLayout pipelineLayout = {}; //管线布局
	VkPipeline graphicsPipeline = {}; //渲染管线
	std::vector<VkFramebuffer> swapChainFramebuffers = {}; //帧缓冲

	void run()
	{
		initWindow(); //初始化窗口
		initVulkan(); //初始化Vulkan
		//printDebug(); //输出调试列表
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

		createSurface(); //表面应在选择物理设备前创建
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();

		createRenderPass();
		createGraphicsPipelines();
		createFramebuffers();
	}

	//创建vulkan容器
	void createInstance();
	//设置校验层及其回调处理
	void setupDebugMessenger();
	//创建窗口表面
	void createSurface();
	//选取物理设备
	void pickPhysicalDevice();
	//创建逻辑设备
	void createLogicalDevice();
	//创建交换链
	void createSwapChain();
	//创建图像视图
	void createImageViews();
	//创建渲染流程
	void createRenderPass();
	//创建渲染管线
	void createGraphicsPipelines();
	//创建帧缓冲
	void createFramebuffers();

	//确定GPU是否合适
	bool isDeviceSuitable(VkPhysicalDevice device);
	//寻找合适的队列簇
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	//获取设备的交换链支持情况
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	//创建着色器模块
	VkShaderModule createShaderModule(const std::vector<char>& code);

	//输出调试列表
	void printDebug();
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
		for (auto imageView : swapChainImageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}
		for (auto framebuffer : swapChainFramebuffers)
		{
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
		vkDestroyRenderPass(device, renderPass, nullptr); //删除渲染流程
		vkDestroyPipeline(device, graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr); //删除管线布局
		vkDestroySwapchainKHR(device, swapChain, nullptr); //删除交换链
		vkDestroyDevice(device, nullptr); //删除逻辑设备
		vkDestroySurfaceKHR(instance, surface, nullptr); //删除表面对象
		vkDestroyInstance(instance, nullptr); //删除vulkan容器
		glfwDestroyWindow(window); //删除window窗口
		glfwTerminate(); //删除所有剩余资源
	}
};

#endif