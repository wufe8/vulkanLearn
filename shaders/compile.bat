@echo off
echo > glslc shader.vert -o vert.spv
glslc shader.vert -o vert.spv
echo > glslc shader.frag -o frag.spv
glslc shader.frag -o frag.spv
pause