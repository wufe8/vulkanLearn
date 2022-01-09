#version 450
//该扩展使着色器代码可以在Vulkan下工作
#extension GL_ARB_separate_shader_objects : enable
//#extension GL_KHR_vulkan_glsl : enable

//注意对于64位变量 会占用不止一个索引位
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

out gl_PerVertex
{
	vec4 gl_Position;
};

//每个顶点均会执行一次main函数 gl_VertexIndex包含当前顶点的索引
void main()
{
	gl_Position = vec4(inPosition, 0.0, 1.0);
	fragColor = inColor;
}
