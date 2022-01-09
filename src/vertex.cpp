#include "frame.h"

void Frame::createVertexBuffer()
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices[0]) * vertices.size();
	//缓冲中数据的使用目的 支持使用位或运算来指定多个使用目的
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	/* sharingMode 访问模式
	* VK_SHARING_MODE_EXCLUSIVE 独占模式 一个缓冲同一时间只能被一个队列簇使用 性能更好
	* VK_SHARING_MODE_CONCURRENT 协同模式 缓冲可以在多个队列簇间使用 */
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.flags = 0; //缓冲的内存稀疏程度 默认0
	//创建顶点缓存对象 注意这并不会创建内存空间 需要自行分配并绑定
	if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	/* VkMemoryRequirements 成员变量:
	* size 需要的内存字节大小 可能与 bufferInfo.size 的值不同
	* alignment 缓冲在实际被分配的内存中的开始位置 依赖于 bufferInfo.usage 和 bufferInfo.flags
	* memoryTypeBits 适合该缓冲使用的内存类型的位域 */
	VkMemoryRequirements memRequirements; //获取缓冲的内存需求
	vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {}; //内存分配信息
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	//分配内存空间 注意不再使用后需要自行释放
	if (vkAllocateMemory(device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("faile to allocate vertex buffer memory!");
	}
	//将内存空间与顶点缓存对象进行绑定 第四个参数位偏移值 需要能被 memrequirements.alignment 整除
	vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);
	//std::cout << "[I] vertex buffer create success" << std::endl;

	void* data;
	//vkMapMemory 给定内存大小和偏移值来访问特定内存资源 用 VK_WHOLE_SIZE 来映射整个申请的内存 倒数第二个参数为标记 目前不可用
	vkMapMemory(device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
	/* 用 memcpy(void*, const void*, size_t) 将顶点数据复制到映射后的内存
	* 注意现代处理器存在缓冲 要求写入内存的数据 驱动程序可能不会立即复制到缓冲关联的内存中去 因此不一定对多个核心同时可见 解决方法:
	* 1. 使用 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 类型的内存可保证一致可见 但相比第二种方法性能稍低
	* 2. 写入数据到映射的内存后调用 vkFlushMappedMemoryRanges 函数 读取映射的内存前调用 vkInvalidateMappeddMemoryRanges 函数 */
	memcpy(data, vertices.data(), size_t(bufferInfo.size));
	vkUnmapMemory(device, vertexBufferMemory); //结束内存映射
}

/* 查找合适的内存类型
* @param typeFilter 过滤的内存类型的位域
* properties 过滤的内存类型:
* VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT 用于从CPU写入数据
* VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 内存一致可见 */
uint32_t Frame::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	/* VkPhysicalDeviceMemoryProperties 成员变量:
	* memoryTypes memoryTypesCount 内存类型
	* memoryHeaps[] memoryHeapsCount 内存来源 比如显存以及显存用尽后位于主内存中的交换空间 */
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties))
		{
			return i; //返回与请求位域及类型均匹配的最小内存位域值
		}
	}
	throw std::runtime_error("failed to find suitable memory type!");
}
