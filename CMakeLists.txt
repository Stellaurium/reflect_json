# 设置 CMake 的最小版本
cmake_minimum_required(VERSION 3.22)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 指定 C 和 C++ 编译器
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)


# Project Name Setting
# 获取项目的根路径的最后一个文件夹的名称
get_filename_component(project_name ${CMAKE_SOURCE_DIR} NAME)
# 空格和-替换为下划线
string(REPLACE " " "_" project_name ${project_name})
string(REPLACE "-" "_" project_name ${project_name})
project(${project_name} CXX)


# Set Program include directory
# my common header
# 自己写的公共头文件的路径，没有可以删除
set(COMMON_INCLUDE_DIRS)
set(PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDE_DIRS} ${PROJECT_INCLUDE_DIRS})


# LIBRARY_OUTPUT_PATH 以及 EXECUTABLE_OUTPUT_PATH 都是旧变量，现在又更新的方式指定
# LIBRARY_OUTPUT_PATH包括了静态库和动态库
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY 是静态库；CMAKE_LIBRARY_OUTPUT_DIRECTORY是动态库
# CMAKE_RUNTIME_OUTPUT_DIRECTORY 是新版的 EXECUTABLE_OUTPUT_PATH
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)


# 为全部的编译项目添加编译参数
# 设计详细的警告 将错误提到编译期
add_compile_options(
        -Wall
        -Wextra
        -Wconversion
        -Wsign-compare
        -Wzero-as-null-pointer-constant
        -Wmissing-declarations
        -Wold-style-cast
        -Wnon-virtual-dtor
        -Werror
)


# find add the cpp file in src directory
file(GLOB_RECURSE source_files ${CMAKE_SOURCE_DIR}/src/*.cpp)


# add required libiary
# 必须一个库一行 不能把所有的库写到一起
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)


# add excutable
add_executable(${project_name} ${source_files})

# 将 第三方库 链接到 可执行文件上
# 这个可以将多个链接写到一行
# 注意boost比较个别 需要使用 ${Boost_LIBRARIES} 链接
target_link_libraries(${project_name} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${project_name} PRIVATE fmt::fmt)


# add excutable
add_executable(${project_name}_test ${CMAKE_SOURCE_DIR}/test/test.cpp)
target_link_libraries(${project_name}_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(${project_name}_test PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${project_name}_test PRIVATE fmt::fmt)

# 为特定目标添加编译选项
# target_compile_options(<TARGET> PRIVATE -Wall -Wextra ...)

# 自定义目标 'run'，它在构建后运行可执行文件
add_custom_target(run
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${project_name}
        DEPENDS ${project_name}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Running ${project_name}..."
        )

# 自定义目标 'run'，它在构建后运行可执行文件
add_custom_target(test
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${project_name}_test
        DEPENDS ${project_name}_test
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Run Testing of ${project_name}..."
        )
