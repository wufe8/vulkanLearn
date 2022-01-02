#include "frame.h"

void Frame::createSurface()
{
	/* //windows��������
	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(window); //���ڵ�ַ
	createInfo.hinstance = GetModuleHandle(nullptr); //����ʵ����ַ
	auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
	if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
	{ //��ȡ����windows���ڱ���ĺ���, KHRָ�ú���Ϊvulkan��չ
		throw std::runtime_error("failed to create window surface!");
	}
	*/
	// linux��������: vkCreateXcbSurfaceKHR

	//ʹ��glfw���п�ƽ̨����
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}
