#version 450
//该扩展使着色器代码可以在Vulkan下工作
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

//每个顶点均会执行一次main函数 gl_VertexIndex包含当前顶点的索引
void main()
{
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
	fragColor = colors[gl_VertexIndex];
}
