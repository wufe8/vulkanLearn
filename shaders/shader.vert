#version 450
//����չʹ��ɫ�����������Vulkan�¹���
#extension GL_ARB_separate_shader_objects : enable
//#extension GL_KHR_vulkan_glsl : enable

//ע�����64λ���� ��ռ�ò�ֹһ������λ
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

out gl_PerVertex
{
	vec4 gl_Position;
};

//ÿ���������ִ��һ��main���� gl_VertexIndex������ǰ���������
void main()
{
	gl_Position = vec4(inPosition, 0.0, 1.0);
	fragColor = inColor;
}
