#递归本文件夹下的cpp文件
file(GLOB_RECURSE APP ./  *.cpp)

add_library(app ${APP})

