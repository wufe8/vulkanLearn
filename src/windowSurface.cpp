#include "frame.h"

void Frame::createSurface()
{
	/* //windows创建方法
	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(window); //窗口地址
	createInfo.hinstance = GetModuleHandle(nullptr); //进程实例地址
	auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
	if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
	{ //获取创建windows窗口表面的函数, KHR指该函数为vulkan扩展
		throw std::runtime_error("failed to create window surface!");
	}
	*/
	// linux创建方法: vkCreateXcbSurfaceKHR

	//使用glfw进行跨平台创建
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}
