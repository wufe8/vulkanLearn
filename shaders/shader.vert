#version 450
//����չʹ��ɫ�����������Vulkan�¹���
#extension GL_ARB_separate_shader_objects : enable
//#extension GL_KHR_vulkan_glsl : enable

out gl_PerVertex
{
	vec4 gl_Position;
};

vec2 positions[3] = vec2[]
(
	vec2(0.0, -0.5),
	vec2(0.5, 0.5),
	vec2(-0.5, 0.5)
);

layout(location = 0) out vec3 fragColor;

vec3 colors[3] = vec3[]
(
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0)
);

//ÿ���������ִ��һ��main���� gl_VertexIndex������ǰ���������
void main()
{
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
	fragColor = colors[gl_VertexIndex];
}
