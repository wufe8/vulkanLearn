#include "frame.h"

void Frame::createRenderPass()
{
	//��������ͼ�����ɫ���帽��
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; //������ ��ʹ�ö��ز�������Ϊ1
	//ָ����Ⱦǰ��Ը����е����ݽ��еĲ���
	/* loadOp ��Ⱦǰ����ɫ����Ȼ���
	* VK_ATTCHMENT_LOAD_OP_LOAD ���ָ��ŵ���������
	* VK_ATTCHMENT_LOAD_OP_CLEAR ʹ��һ������ֵ�������������
	* VK_ATTCHMENT_LOAD_OP_DONT_CARE �������ŵ��������� */
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	/* storeOp ��Ⱦ�����ɫ����Ȼ���
	* VK_ATTCHMENT_LOAD_OP_STORE ��Ⱦ�����ݽ����洢�Ա�֮���ȡ
	* VK_ATTCHMENT_LOAD_OP_DONT_CARE ��Ⱦ�󲻶�ȡ֡�������� */
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//loadOp ��Ⱦǰ��ģ�建��
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//storeOp ��Ⱦ���ģ�建��
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	/* initialLayout/finalLayout ��Ⱦ���̿�ʼǰ/�������ͼ�񲼾ַ�ʽ
	* VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ͼ��������ɫ����
	* VK_IMAGE_LAYOUT_PRESENT_SRC_KHR ͼ�����ڽ������н��г��ֲ���
	* VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ͼ���������Ʋ�����Ŀ��ͼ��
	* VK_IMAGE_LAYOUT_UNDEFINED δ���� ������ ����֤ͼ�����ݻᱻ���� */
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	//�����̸�������
	VkAttachmentReference colorAttachmentRef = {};
	//���õĸ����ڸ��������ṹ�������е�����
	colorAttachmentRef.attachment = 0;
	//���������������õĸ���ʹ�õĲ��ַ�ʽ vulkan���������̿�ʼʱ�Զ������õĸ���ת������ͼ�񲼾�
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	//��ʽָ������һ��ͼ����Ⱦ��������
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef; // ����ɫ�����������е������ᱻƬ����ɫ��ʹ�� ��Ӧ layout(location = 0) out vec4 outColor
	subpass.pInputAttachments = nullptr; //����ɫ����ȡ�ĸ���
	subpass.pResolveAttachments = nullptr; //���ڶ��ز�������ɫ����
	subpass.pDepthStencilAttachment = nullptr; //������Ⱥ�ģ�����ݵĸ���
	subpass.pPreserveAttachments = nullptr; //������������ʹ�� ���Ա������ݵĸ���

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass!");
	}

}