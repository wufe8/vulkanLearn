#ifndef FRAME_H
#define FRAME_H
#pragma once

#define GLFW_INCLUDE_VULKAN //����glfwʹ��vulkan api
#include <GLFW/glfw3.h> //glfw���ڴ�������

#include <iostream> //iostream���ڿ���̨���
#include <stdexcept> //stdexcept֧�ֶ��ִ������͵Ĵ���
#include <functional> //functional������Դ����
#include <cstdlib> //cstdlibʹ��EXIT_SUCESS, EXIT_FAILURE��
#include <vector> //vector���ݽṹ

const int WIDTH = 800; //���峤��
const int HEIGHT = 600;

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

//���vulkan��չ�б�
void printVkExtensionList();
//�������У����б�
void printVkLayerList();
//������У���
bool checkValidationLayerSupport();
/* TODO ��������豸
* @param instance vulkan���� */
void printPhysicalDevices(VkInstance instance);
//��ȡ��չ
std::vector<const char*> getRequiredExtensions();

//���д���Ŵ洢������Ч�Լ��
struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	bool isComplete()
	{
		return graphicsFamily >= 0;
	}
};
//���Ҷ��д�
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

//������ ��ȡɾ�����Բ�ص�����ĺ���
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
//У�����Զ�����Ϣ����, ����vulkan��������pNext
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

class Frame //��������
{
public:
	GLFWwindow* window = VK_NULL_HANDLE; //���ڶ���
	//GLFWwindow* subwindow;
	VkInstance instance = {}; //vulkan����
	VkDebugUtilsMessengerEXT debugMessenger = {}; //�ص�������Ϣ
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; //�����豸
	VkDevice device = VK_NULL_HANDLE; //�߼��豸
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	void run()
	{
		initWindow(); //��ʼ������
		initVulkan(); //��ʼ��Vulkan
		mainLoop(); //��Ⱦ�¼�ѭ��
		cleanup(); //����ǰ������Դ
	}
	Frame()
	{
		window = nullptr;
	}

private:
	virtual void initWindow()
	{
		glfwInit(); //��ʼ��glfw
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //��ֹglfw����OpenGL(Ĭ�����)
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //��ֹ�������ڴ�С
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //�������� ����ȡ��ַ
		//subwindow = glfwCreateWindow(WIDTH - 200, HEIGHT - 200, "subwindow", nullptr, nullptr);
	}

	virtual void initVulkan()
	{
		createInstance();
		setupDebugMessenger();
		pickPhysicalDevice();
		createLogicalDevice();
	}

	//����vulkan����
	void createInstance();
	//����У��㼰��ص�����
	void setupDebugMessenger();
	//ѡȡ�����豸
	void pickPhysicalDevice();
	//�����߼��豸
	void createLogicalDevice();

	virtual void mainLoop()
	{
		while (!glfwWindowShouldClose(window)) //ѭ��ֱ������ر�window����
		{
			glfwPollEvents(); //�����¼�����
		}
	}

	virtual void cleanup()
	{
		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}
		vkDestroyDevice(device, nullptr); //ɾ���߼��豸
		vkDestroyInstance(instance, nullptr); //ɾ��vulkan����
		glfwDestroyWindow(window); //ɾ��window����
		glfwTerminate(); //ɾ������ʣ����Դ
	}
};

#endif