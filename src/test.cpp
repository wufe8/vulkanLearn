#define GLFW_INCLUDE_VULKAN //声明glfw使用vulkan api
#include <GLFW/glfw3.h> //glfw用于创建窗口

#include <iostream> //iostream用于控制台输出
#include <stdexcept> //stdexcept支持多种错误类型的处理
#include <functional> //functional用于资源管理
#include <cstdlib> //cstdlib使用EXIT_SUCESS, EXIT_FAILURE宏

const int WIDTH = 800; //定义长宽
const int HEIGHT = 600;

class HelloTriangleApplication //主程序类
{
public:
	GLFWwindow* window; //窗口定义
	//GLFWwindow* subwindow;
	void run()
	{
		initWindow(); //初始化窗口
		initVulkan(); //初始化Vulkan
		mainLoop(); //渲染事件循环
		cleanup(); //结束前清理资源
	}

private:
	void initWindow()
	{
		glfwInit(); //初始化glfw
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //阻止glfw调用OpenGL(默认情况)
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //禁止调整窗口大小
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //创建窗口 并获取地址
		//subwindow = glfwCreateWindow(WIDTH - 200, HEIGHT - 200, "subwindow", nullptr, nullptr);
	}

	void initVulkan()
	{

	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window)) //循环直到点击关闭window窗口
		{
			glfwPollEvents(); //推送事件处理
		}
	}

	void cleanup()
	{
		glfwDestroyWindow(window); //删除window窗口
		glfwTerminate(); //删除所有剩余资源
	}
};

int main() {
	HelloTriangleApplication app; //初始化主窗口类
	try
	{
		app.run();
	}
	catch (const std::exception& e) //捕获错误
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
