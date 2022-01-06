#ifndef FRAME_H
#define FRAME_H
#pragma once

//#define VK_USE_PLATFORM_WIN32_KHR //ʹ��windowsƽ̨ԭ������
#define GLFW_INCLUDE_VULKAN //����glfwʹ��vulkan api
#include <GLFW/glfw3.h> //glfw���ڴ�������
//#define GLFW_EXPOSE_NATIVE_WIN32 //ʹ��windowsƽ̨ԭ������
//#include <GLFW/glfw3native.h> //ʹ��ƽ̨ԭ��

#include <iostream> //iostream���ڿ���̨���
#include <stdexcept> //stdexcept֧�ֶ��ִ������͵Ĵ���
#include <functional> //functional������Դ����
#include <cstdlib> //cstdlibʹ��EXIT_SUCESS, EXIT_FAILURE��
#include <vector> //vector���ݽṹ
#include <set> //set���ݽṹ
#include <algorithm> //��ѧ����Ƚ�
#include <fstream> //�ļ�����

const int WIDTH = 800; //���峤��
const int HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2; //��ͬʱ���д����֡��

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation" //��ʽ�������п���У���
};

#ifdef NDEBUG //debugģʽ���뽫���ü����
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

//�����Ϣ����ȼ�
const VkDebugUtilsMessageSeverityFlagBitsEXT DebugLevel =
VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

class Frame;

//���vulkan��չ�б�
void printVkExtensionList();
//�������У����б�
void printVkLayerList();
/* TODO ��������豸
* @param instance vulkan���� */
void printPhysicalDevices(Frame* pFrame);
//����豸��չ�б�
void printDeviceExtensionList(Frame* pFrame);
//��ȡ�������ļ�
static std::vector<char> readFileBinary(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}
	//��Ϊ��β����ʼ��ȡ ��˿��ɶ�ȡλ�õ�֪�ļ���С �����������ռ�
	size_t fileSize = size_t(file.tellg());
	std::vector<char> buffer(fileSize);
	file.seekg(0); //�����ļ�ͷ
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

//������У���
bool checkValidationLayerSupport();
//��齻����֧�����
//bool querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);


//��ȡ������չ
std::vector<const char*> getRequiredInstanceExtensions();
//��ȡ�豸��չ
std::vector<const char*> getRequiredDeviceExtensions();

//���д���Ŵ洢������Ч�Լ��
struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily  = -1;
	bool isComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

/*������֧��ϸ��
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

//������ ��ȡɾ�����Բ�ص�����ĺ���
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
//У�����Զ�����Ϣ����, ����vulkan��������pNext
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

//���ڴ�С�ı�ʱӦ���øûص�����
static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

class Frame //��������
{
public:
	GLFWwindow* window = VK_NULL_HANDLE; //���ڶ���
	//GLFWwindow* subwindow;
	VkInstance instance = {}; //vulkan����
	VkDebugUtilsMessengerEXT debugMessenger = {}; //�ص�������Ϣ
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; //�����豸 �������
	VkDevice device = VK_NULL_HANDLE; //�߼��豸
	VkQueue graphicsQueue = VK_NULL_HANDLE; //ͼ����� ����߼��豸ʱ�Զ����
	VkQueue presentQueue = VK_NULL_HANDLE; //���ֶ��� ����߼��豸ʱ�Զ����
	VkSurfaceKHR surface = VK_NULL_HANDLE; //���ڱ������
	VkSwapchainKHR swapChain = VK_NULL_HANDLE; //������
	std::vector<VkImage> swapChainImages = {}; //������ͼ�� ���������ʱ�Զ����
	VkFormat swapChainImageFormat = {}; //������ͼ���ʽ
	VkExtent2D swapChainExtent = {}; //������ͼ��ֱ���
	std::vector<VkImageView> swapChainImageViews = {}; //ͼ����ͼ
	VkRenderPass renderPass = {}; //��Ⱦ����
	VkPipelineLayout pipelineLayout = {}; //���߲���
	VkPipeline graphicsPipeline = {}; //��Ⱦ����
	std::vector<VkFramebuffer> swapChainFramebuffers = {}; //֡����
	VkCommandPool commandPool = {}; //ָ���
	std::vector<VkCommandBuffer> commandBuffers = {}; //ָ���
	std::vector<VkSemaphore> imageAvailableSemaphore = {}; //�ź���--ͼ���ѻ�ȡ
	std::vector<VkSemaphore> renderFinishedSemaphore = {}; //�ź���--��Ⱦ���
	std::vector<VkFence> inFlightFences = {}; //CPU��GPU��ͬ�� ��ֹ����MAX_FRAMES_IN_FLIGHT֡��ָ��ͬʱ���ύִ��
	size_t currentFrame = 0; //��ǰ��Ⱦ��֡ ����ѡ��ǰ֡Ӧ��ʹ�õ��ź���
	bool framebufferResized = false; //���֡����(����)��С�Ƿ��Ѿ��ı�

	void run()
	{
		initWindow(); //��ʼ������
		initVulkan(); //��ʼ��Vulkan
		//printDebug(); //��������б�
		mainLoop(); //��Ⱦ�¼�ѭ��
		cleanup(); //����ǰ������Դ
	}
	Frame()
	{
		window = nullptr;
	}

	//��������б�
	void printDebug();

private:
	virtual void initWindow()
	{
		glfwInit(); //��ʼ��glfw
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //��ֹglfw����OpenGL(Ĭ�����)
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //��ֹ�������ڴ�С
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //�������� ����ȡ��ַ
		//subwindow = glfwCreateWindow(WIDTH - 200, HEIGHT - 200, "subwindow", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	virtual void initVulkan()
	{
		createInstance();
		setupDebugMessenger();

		createSurface(); //����Ӧ��ѡ�������豸ǰ����
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();

		createRenderPass();
		createGraphicsPipelines();
		createFramebuffers();
		createCommandPool();
		createCommandBuffers();
		createSyncObjects();
	}

	//����vulkan����
	void createInstance();
	//����У��㼰��ص�����
	void setupDebugMessenger();
	//�������ڱ���
	void createSurface();
	//ѡȡ�����豸
	void pickPhysicalDevice();
	//�����߼��豸
	void createLogicalDevice();
	//����������
	void createSwapChain();
	//ѡ�񽻻���Χ(��������ͼ��ķֱ���) ��Ҫ��ȡ���ڶ��������ؽ�ʱ��Ӧ���ڳߴ�ı仯
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR capabilities);
	//�����������ض��� Ϊ�ؽ���������׼��
	void cleanupSwapChain();
	//�ؽ������� Ӧ���細�ڴ�С�ı�Ƚ������봰�ڲ���ƥ������
	void recreateSwapChain();
	//����ͼ����ͼ
	void createImageViews();
	//������ȾVK_SUBPASS_EXTERNAL��������ִ��ǰ�����(���������������)����
	void createRenderPass();
	//������Ⱦ����
	void createGraphicsPipelines();
	//����֡����
	void createFramebuffers();
	//����ָ���
	void createCommandPool();
	//����ָ���
	void createCommandBuffers();
	//�����ź���
	void createSyncObjects();

	//ȷ��GPU�Ƿ����
	bool isDeviceSuitable(VkPhysicalDevice device);
	//Ѱ�Һ��ʵĶ��д�
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	//��ȡ�豸�Ľ�����֧�����
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	//������ɫ��ģ��
	VkShaderModule createShaderModule(const std::vector<char>& code);

	virtual void mainLoop()
	{
		while (!glfwWindowShouldClose(window)) //ѭ��ֱ������ر�window����
		{
			glfwPollEvents(); //�����¼�����
			drawFrame(); //����ͼ��
		}
		vkDeviceWaitIdle(device); //�ȴ��豸������� ������������������ͻ
	}

	/* ����ͼ��
	* 1. �ӽ�������ȡһ��ͼ��
	* 2. ��֡���帽��ִ��ָ����е���Ⱦָ��
	* 3. ������Ⱦ���ͼ�񵽽��������г��ֲ���
	* ʵ��ִ�о�Ϊ�첽���� ��ͨ��դ��(fence)���ź���(semaphore)����ͬ��
	* ���� vkWaitForFences ������ѯfence״̬ ���޷���ѯsemaphore״̬
	* ͨ����fence��Ӧ�ó��������Ⱦ��������ͬ��
	* ��semaphore��һ��ָ������ڵĲ���������ָͬ����еĲ�������ͬ��
	* */
	void drawFrame(); 

	virtual void cleanup()
	{
		cleanupSwapChain(); //�����������ض���(������, ͼ����ͼ, ��Ⱦ����, ���߲���, ��Ⱦ����, ֡����)
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(device, renderFinishedSemaphore[i], nullptr); //����ź���
			vkDestroySemaphore(device, imageAvailableSemaphore[i], nullptr);
			vkDestroyFence(device, inFlightFences[i], nullptr);
		}
		vkDestroyCommandPool(device, commandPool, nullptr); //���ָ���
		vkDestroyDevice(device, nullptr); //����߼��豸
		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}
		vkDestroySurfaceKHR(instance, surface, nullptr); //����������
		vkDestroyInstance(instance, nullptr); //���vulkan����
		glfwDestroyWindow(window); //���window����
		glfwTerminate(); //�������ʣ����Դ
	}
};

//���ڴ�С�ı�ʱӦ���øûص�����
static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(window));
	frame->framebufferResized = true;
}

#endif