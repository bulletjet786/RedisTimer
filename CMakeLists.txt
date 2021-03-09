cmake_minimum_required(VERSION 3.17)
project(RedisLeague C)

set(CMAKE_C_STANDARD 99)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) #设置可执行文件保存的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)	#设置链接库保存的路径

add_subdirectory(hello)
add_subdirectory(dinger)