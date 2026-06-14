cmake_minimum_required(VERSION 3.12)

project(OpenGL_lecture)

set(CMAKE_CXX_STANDARD 17)

file(GLOB ASSETS "./assets")
file(COPY ${ASSETS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


#往项目中加入一个全局的预编译宏 相当于一个开关
add_definitions(-DDEBUG)


include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)
#此处是标记处理头文件的路径，其中，CMAKE_CURRENT_SOURCE_DIR是当前CMakeLists.txt所在的路径，在本项目中是根目录
#经过该处理，可以在main.cpp中直接#include "GLFW/glfw.h"等

#lib库在连接的时候加入到本项目中
link_directories(   
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib	
)

add_subdirectory(wrapper)#需要放在include下面，include的作用是规定头文件的搜索路径
add_subdirectory(application)
add_subdirectory(glframework)
add_subdirectory(imgui)


add_executable(openglStudy "main.cpp""glad.c" )#每次拷贝都会自动加入一些项目，记得删除
#注意这里一定要有一个空格↑
#为什么每次加入一个新的文件夹，文件夹中的.h.cpp文件都会自动加入到可执行文件中？

target_link_libraries(openglStudy
    PRIVATE
    glfw3
    assimp-vc143-mtd.lib
    zlibstaticd.lib
    wrapper
    app
    fw
    imguilib
)


#target_link_libraries(openglStudy glfw3.lib wrapper app fw)#target必须放在底部（？