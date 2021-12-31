cmake_minimum_required (VERSION 3.8)
set(CMAKE_C_SRANDARD 11)
set(CMAKE_CXX_SRANDARD 17)

include_directories(C:/VulkanSDK/1.2.198.1/Include C:/VulkanSDK/1.2.198.1/Third-Party/Include)
link_directories(C:/VulkanSDK/1.2.198.1/Lib C:/VulkanSDK/1.2.198.1/Third-Party/Bin)

project(Test)
add_subdirectory(src bin) #添加子目录src文件夹 以及指定编译输出至bin文件夹
#add_subdirectory(lib lib)
#subdirs(src) 可单行指定多个子目录 但无法指定编译输出目录

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) #重定义二进制文件输出位置
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) #重定义生成库位置
#如果需要改变目标存放路径 那么在哪里 ADD_EXECUTABLE 或 ADD_LIBRARY 就在哪里加入上述的定义

message(STATUS "BINARY dir: " ${PROJECT_BINARY_DIR})
message(STATUS "SOURCE dir: " ${PROJECT_SOURCE_DIR})