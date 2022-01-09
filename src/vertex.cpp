#include "frame.h"

void Frame::createVertexBuffer()
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices[0]) * vertices.size();
	//���������ݵ�ʹ��Ŀ�� ֧��ʹ��λ��������ָ�����ʹ��Ŀ��
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	/* sharingMode ����ģʽ
	* VK_SHARING_MODE_EXCLUSIVE ��ռģʽ һ������ͬһʱ��ֻ�ܱ�һ�����д�ʹ�� ���ܸ���
	* VK_SHARING_MODE_CONCURRENT Эͬģʽ ��������ڶ�����дؼ�ʹ�� */
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.flags = 0; //������ڴ�ϡ��̶� Ĭ��0
	//�������㻺����� ע���Ⲣ���ᴴ���ڴ�ռ� ��Ҫ���з��䲢��
	if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	/* VkMemoryRequirements ��Ա����:
	* size ��Ҫ���ڴ��ֽڴ�С ������ bufferInfo.size ��ֵ��ͬ
	* alignment ������ʵ�ʱ�������ڴ��еĿ�ʼλ�� ������ bufferInfo.usage �� bufferInfo.flags
	* memoryTypeBits �ʺϸû���ʹ�õ��ڴ����͵�λ�� */
	VkMemoryRequirements memRequirements; //��ȡ������ڴ�����
	vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {}; //�ڴ������Ϣ
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	//�����ڴ�ռ� ע�ⲻ��ʹ�ú���Ҫ�����ͷ�
	if (vkAllocateMemory(device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("faile to allocate vertex buffer memory!");
	}
	//���ڴ�ռ��붥�㻺�������а� ���ĸ�����λƫ��ֵ ��Ҫ�ܱ� memrequirements.alignment ����
	vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);
	//std::cout << "[I] vertex buffer create success" << std::endl;

	void* data;
	//vkMapMemory �����ڴ��С��ƫ��ֵ�������ض��ڴ���Դ �� VK_WHOLE_SIZE ��ӳ������������ڴ� �����ڶ�������Ϊ��� Ŀǰ������
	vkMapMemory(device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
	/* �� memcpy(void*, const void*, size_t) ���������ݸ��Ƶ�ӳ�����ڴ�
	* ע���ִ����������ڻ��� Ҫ��д���ڴ������ ����������ܲ����������Ƶ�����������ڴ���ȥ ��˲�һ���Զ������ͬʱ�ɼ� �������:
	* 1. ʹ�� VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ���͵��ڴ�ɱ�֤һ�¿ɼ� ����ȵڶ��ַ��������Ե�
	* 2. д�����ݵ�ӳ����ڴ����� vkFlushMappedMemoryRanges ���� ��ȡӳ����ڴ�ǰ���� vkInvalidateMappeddMemoryRanges ���� */
	memcpy(data, vertices.data(), size_t(bufferInfo.size));
	vkUnmapMemory(device, vertexBufferMemory); //�����ڴ�ӳ��
}

/* ���Һ��ʵ��ڴ�����
* @param typeFilter ���˵��ڴ����͵�λ��
* properties ���˵��ڴ�����:
* VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ���ڴ�CPUд������
* VK_MEMORY_PROPERTY_HOST_COHERENT_BIT �ڴ�һ�¿ɼ� */
uint32_t Frame::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	/* VkPhysicalDeviceMemoryProperties ��Ա����:
	* memoryTypes memoryTypesCount �ڴ�����
	* memoryHeaps[] memoryHeapsCount �ڴ���Դ �����Դ��Լ��Դ��þ���λ�����ڴ��еĽ����ռ� */
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties))
		{
			return i; //����������λ�����;�ƥ�����С�ڴ�λ��ֵ
		}
	}
	throw std::runtime_error("failed to find suitable memory type!");
}
