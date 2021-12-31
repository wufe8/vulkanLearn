#define GLFW_INCLUDE_VULKAN //����glfwʹ��vulkan api
#include <GLFW/glfw3.h> //glfw���ڴ�������

#include <iostream> //iostream���ڿ���̨���
#include <stdexcept> //stdexcept֧�ֶ��ִ������͵Ĵ���
#include <functional> //functional������Դ����
#include <cstdlib> //cstdlibʹ��EXIT_SUCESS, EXIT_FAILURE��

const int WIDTH = 800; //���峤��
const int HEIGHT = 600;

class HelloTriangleApplication //��������
{
public:
	GLFWwindow* window; //���ڶ���
	//GLFWwindow* subwindow;
	void run()
	{
		initWindow(); //��ʼ������
		initVulkan(); //��ʼ��Vulkan
		mainLoop(); //��Ⱦ�¼�ѭ��
		cleanup(); //����ǰ������Դ
	}

private:
	void initWindow()
	{
		glfwInit(); //��ʼ��glfw
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //��ֹglfw����OpenGL(Ĭ�����)
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //��ֹ�������ڴ�С
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //�������� ����ȡ��ַ
		//subwindow = glfwCreateWindow(WIDTH - 200, HEIGHT - 200, "subwindow", nullptr, nullptr);
	}

	void initVulkan()
	{

	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window)) //ѭ��ֱ������ر�window����
		{
			glfwPollEvents(); //�����¼�����
		}
	}

	void cleanup()
	{
		glfwDestroyWindow(window); //ɾ��window����
		glfwTerminate(); //ɾ������ʣ����Դ
	}
};

int main() {
	HelloTriangleApplication app; //��ʼ����������
	try
	{
		app.run();
	}
	catch (const std::exception& e) //�������
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
