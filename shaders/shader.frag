#version 450
#extension GL_ARB_separate_shader_objects : enable

//同一组in out可以使用不同名称 编译器通过location对应
layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

//对应RGBA 取值范围[0,1]
void main()
{
	outColor = vec4(fragColor, 1.0);
}
