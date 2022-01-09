#ifndef FRAME_H
#define FRAME_H
#pragma once

//#define VK_USE_PLATFORM_WIN32_KHR //使用windows平台原生功能
#define GLFW_INCLUDE_VULKAN //声明glfw使用vulkan api
#include <GLFW/glfw3.h> //glfw用于创建窗口
//#define GLFW_EXPOSE_NATIVE_WIN32 //使用windows平台原生功能
//#include <GLFW/glfw3native.h> //使用平台原生
#include <glm/glm.hpp>

#include <iostream> //iostream用于控制台输出
#include <stdexcept> //stdexcept支持多种错误类型的处理
#include <functional> //functional用于资源管理
#include <cstdlib> //cstdlib使用EXIT_SUCESS, EXIT_FAILURE宏
#include <vector> //vector数据结构
#include <array> //array数据结构
#include <set> //set数据结构
#include <algorithm> //数学运算比较
#include <fstream> //文件处理

const int WIDTH = 800; //定义长宽
const int HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2; //可同时并行处理的帧数

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

//窗口大小改变时应调用该回调函数
static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

//交叉顶点属性(顶点位置与顶点颜色数据定义在同一个结构体数组中)
struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	//获取顶点信息
	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		//绑定数组中绑定的索引
		bindingDescription.binding = 0;
		//一个条目到下一个条目的字节数
		bindingDescription.stride = sizeof(Vertex);
		/* 遍历方式
		* VK_VERTEX_INPUT_RATE_VERTEX 每次移动到顶点后的下一个数据条目
		* VK_VERTEX_INPUT_RATE_VERTEX 每次移动到容器后的下一个数据条目 */
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	//获取属性信息
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		//描述位置
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		/* 数据类型(均用类似颜色的名字) 隐式定义了属性数据的字节大小 对于着色器类型:
		* float VK_FORMAT_R32_SFLOAT
		* vec2 VK_FORMAT_R32G32_SFLOAT
		* vec3 VK_FORMAT_R32G32B32_SFLOAT
		* vec4 VK_FORMAT_R32G32B32A32_SFLOAT
		* 另有以下变种:
		* ivec2 VK_FORMAT_R32G32_SINT 2个分量的32位有符号整型向量
		* uvec4 VK_FORMAT_R32G32B32A32_UINT 4个分量的32位无符号整型向量
		* double VK_FORMAT_R64_SFLOAT 双精度浮点 */
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);
		//描述颜色
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);
		return attributeDescriptions;
	}
};

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
	VkCommandPool commandPool = {}; //指令池
	std::vector<VkCommandBuffer> commandBuffers = {}; //指令缓冲
	std::vector<VkSemaphore> imageAvailableSemaphores = {}; //信号量--图像已获取
	std::vector<VkSemaphore> renderFinishedSemaphores = {}; //信号量--渲染完成
	std::vector<VkFence> inFlightFences = {}; //CPU和GPU的同步 防止超过MAX_FRAMES_IN_FLIGHT帧的指令同时被提交执行
	std::vector<VkFence> imagesInFlight = {}; //跟踪交换链图像是否有正在使用的帧
	size_t currentFrame = 0; //当前渲染的帧 用来选择当前帧应该使用的信号量
	bool framebufferResized = false; //标记帧缓冲(窗口)大小是否已经改变
	VkBuffer vertexBuffer; //顶点缓冲
	VkDeviceMemory vertexBufferMemory; //顶点缓冲内存空间

	const std::vector<Vertex> vertices =
	{
		{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

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

	//输出调试列表
	void printDebug();

private:
	virtual void initWindow()
	{
		glfwInit(); //初始化glfw
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //阻止glfw调用OpenGL(默认情况)
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //禁止调整窗口大小
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //创建窗口 并获取地址
		//subwindow = glfwCreateWindow(WIDTH - 200, HEIGHT - 200, "subwindow", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback); //绑定改变窗口尺寸时的回调函数
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
		createCommandPool();
		createVertexBuffer();
		createCommandBuffers();
		createSyncObjects();
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
	//选择交换范围(交换链中图像的分辨率) 需要获取窗口对象以在重建时适应窗口尺寸的变化
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities);
	//清除交换链相关对象 为重建交换链做准备
	void cleanupSwapChain();
	//重建交换链 应对如窗口大小改变等交换链与窗口不再匹配的情况
	void recreateSwapChain();
	//创建图像视图
	void createImageViews();
	//创建渲染VK_SUBPASS_EXTERNAL即子流程执行前后操作(这个隐含的子流程)流程
	void createRenderPass();
	//创建渲染管线
	void createGraphicsPipelines();
	//创建帧缓冲
	void createFramebuffers();
	//创建指令池
	void createCommandPool();
	//创建顶点缓冲
	void createVertexBuffer();
	//创建指令缓冲
	void createCommandBuffers();
	//创建信号量
	void createSyncObjects();

	//确定GPU是否合适
	bool isDeviceSuitable(VkPhysicalDevice device);
	//寻找合适的队列簇
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	//获取设备的交换链支持情况
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	//创建着色器模块
	VkShaderModule createShaderModule(const std::vector<char>& code);
	//查找最合适的内存类型
	uint32_t Frame::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	virtual void mainLoop()
	{
		while (!glfwWindowShouldClose(window)) //循环直到点击关闭window窗口
		{
			glfwPollEvents(); //推送事件处理
			drawFrame(); //绘制图像
		}
		vkDeviceWaitIdle(device); //等待设备绘制完成 避免与后续清除操作冲突
	}

	/* 绘制图像
	* 1. 从交换链获取一张图像
	* 2. 对帧缓冲附着执行指令缓冲中的渲染指令
	* 3. 返回渲染后的图像到交换链进行呈现操作
	* 实际执行均为异步操作 可通过栅栏(fence)或信号量(semaphore)进行同步
	* 可用 vkWaitForFences 函数查询fence状态 但无法查询semaphore状态
	* 通常用fence对应用程序本身和渲染操作进行同步
	* 用semaphore对一个指令队列内的操作或多个不同指令队列的操作进行同步
	* */
	void drawFrame(); 

	virtual void cleanup()
	{
		cleanupSwapChain(); //清除交换链相关对象(交换链, 图像视图, 渲染流程, 管线布局, 渲染管线, 帧缓冲)
		vkDestroyBuffer(device, vertexBuffer, nullptr); //清除顶点缓冲
		vkFreeMemory(device, vertexBufferMemory, nullptr);
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr); //清除信号量
			vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(device, inFlightFences[i], nullptr);
		}
		//不清除报错未清除 清除报错报错fence使用中 暂时无法解决
		//for (size_t i = 0; i < imagesInFlight.size(); i++)
		//{
		//	vkDestroyFence(device, imagesInFlight[i], nullptr);
		//}
		vkDestroyCommandPool(device, commandPool, nullptr); //清除指令池
		vkDestroyDevice(device, nullptr); //清除逻辑设备
		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}
		vkDestroySurfaceKHR(instance, surface, nullptr); //清除表面对象
		vkDestroyInstance(instance, nullptr); //清除vulkan容器
		glfwDestroyWindow(window); //清除window窗口
		glfwTerminate(); //清除所有剩余资源
	}
};

//窗口大小改变时应调用该回调函数
static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(window));
	frame->framebufferResized = true;
}

#endif