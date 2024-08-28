# CMakeList.txt: 顶层 CMake 项目文件
cmake_minimum_required (VERSION 3.8)

project (veGameDemo CXX C)

# 使用MSVC编译器时添加/utf-8选项，以确保编译器正确地处理UTF-8编码的源文件
add_compile_options("/utf-8")

# 增加clourenderx cmake模块的寻找路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cloudrenderx_x64-debug/cmake")
# 增加三方库的查询路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/third_parts")

# 查找cloudrenderx模块
find_package(CloudRenderX REQUIRED)
# 查找spdlog模块
find_package(SpdLog REQUIRED)

# 添加Demo模块
file(GLOB DEMO_SOURCE_FILES 
"${CMAKE_CURRENT_LIST_DIR}/veGameDemo/*.cpp"
"${CMAKE_CURRENT_LIST_DIR}/veGameDemo/common/*.cpp"
"${CMAKE_CURRENT_LIST_DIR}/veGameDemo/configs/*.cpp"
"${CMAKE_CURRENT_LIST_DIR}/veGameDemo/demos/*.cpp"
"${CMAKE_CURRENT_LIST_DIR}/veGameDemo/demos/quick_demo/*.cpp"   # 快速开始demo
"${CMAKE_CURRENT_LIST_DIR}/veGameDemo/session/*.cpp")

add_executable(veGameDemo 
	WIN32 
	${DEMO_SOURCE_FILES})

# 增加demo对cloudrenderx相关库的依赖
target_link_libraries(veGameDemo ${CloudRenderXLibraries})
# 增加demo对日志库的依赖
target_link_libraries(veGameDemo SpdLog UxTheme.lib )

include_directories("${CMAKE_CURRENT_LIST_DIR}/veGameDemo")
include_directories("${CMAKE_CURRENT_LIST_DIR}/veGameDemo/common")

# 将config.ini文件 copy到执行目录
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/veGameDemo/config.ini)
    file(COPY ${CMAKE_CURRENT_LIST_DIR}/veGameDemo/config.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()