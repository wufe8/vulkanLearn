#include "frame.h"

void Frame::drawFrame()
{
	//等待fence发出信号 第四个参数指定等待全部fence(VK_TRUE)还是任一fence(VK_FALSE)
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &inFlightFences[currentFrame]); //重置fence为未发出信号的状态
	uint32_t imageIndex;
	/* 从交换链获取一张图像 (交换链是扩展特性 因此与之相关的函数名均带KHR后缀)
	* 参数:
	* VkDevice device 逻辑设备对象
    * VkSwapchainKHR swapchain 要获取图像的交换链
    * uint64_t timeout 图像获取的超时时间 用无符号64位整型的最大值(UINT64_MAX)表示没有超时时间
    * VkSemaphore semaphore 图像可用后通知的信号量对象
    * VkFence fence 图像可用后通知的栅栏对象
    * uint32_t* pImageIndex 输出可用交换链图像的索引 用以引用 VkImage对象

	* */
	vkAcquireNextImageKHR(
		device,
		swapChain,
		UINT64_MAX,
		imageAvailableSemaphore[currentFrame],
		VK_NULL_HANDLE,
		&imageIndex);
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT  可写入颜色附着的管线阶段
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	//队列开始执行前需要等待的信号量及其数量
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore[currentFrame] };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	//队列开始执行前需要等待的管线阶段
	submitInfo.pWaitDstStageMask = waitStages;
	//被提交执行的指令缓冲对象及其数量
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	//指定指令缓冲执行结束后发出信号的信号量对象及其数量
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	/* 提交指令缓冲给图形指令队列 第二个参数是提交数量(支持同时提交多个) 最后一个参数是指令缓冲执行结束后通知的栅栏对象
	*/
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}
	/* 渲染流程开始和结束时会自动进行图像布局变换 但变化发生在管线开始时还没有获取到交换链图像 需配置子流程依赖解决
	* 子程序依赖配置见"renderPass.cpp" */

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	//指定开始呈现操作需要等待的信号量
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain };
	//用于呈现图像的交换链
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	//需要呈现的图像在交换链中的索引
	presentInfo.pImageIndices = &imageIndex;
	//获取每个交换链的呈现操作是否成功的信息 只有一个交换链时可用直接使用呈现函数的返回值来判断
	presentInfo.pResults = nullptr;
	//呈现
	vkQueuePresentKHR(presentQueue, &presentInfo);
	//等待上一次提交的指令结束执行 防止指令堆积(内存泄露) 但会浪费GPU资源
	//vkQueueWaitIdle(presentQueue);
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT; //用模运算使其在 0 到 MAX_FRAMES_IN_FLIGHT - 1 之间循环
}