#version 450
#extension GL_ARB_separate_shader_objects : enable

//ͬһ��in out����ʹ�ò�ͬ���� ������ͨ��location��Ӧ
layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

//��ӦRGBA ȡֵ��Χ[0,1]
void main()
{
	outColor = vec4(fragColor, 1.0);
}
