cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "BuriedPoint")
project(${PROJECT_NAME} VERSION 0.1.1)                     # CMake项目根目录的CMakeLists.txt都需要有这个

option(BUILD_BURIED_SHARED_LIBS "build shared library" ON) # 自定义开关选项，是否编译动态库(默认打开状态)
option(BUILD_BURIED_STATIC_LIBS "build static library" ON) 

option(BUILD_BURIED_EXAMPLES "build examples" OFF)         # 是否编译示例代码程序
option(BUILD_BURIED_TEST "build unittest" OFF)             # 是否编译单元测试程序

# 用于配置MSVC（Microsoft Visual C++）编译器的编译选项，以支持多线程编译
# 提供了一个选项来选择是否构建为多线程运行时库（/MT）
option(BUILD_BURIED_FOR_MT "build for /MT" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")       # MSVC 编译专用的配置，表示启动多线程编译(并行编译源文件，以加速构建过程)，如果是Linux下，这个需要注释掉
if(BUILD_BURIED_FOR_MT)                             # 分别为Debug和Relese构建类型设置编译器选项，以确保生成的可执行文件链接到正确的运行时库
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

# 设置所依赖的库，win平台 相当于 libs = libs + winmm + iphlpapi + ws2_32 + dbghelp   
set(LIBS ${LIBS} winmm iphlpapi ws2_32 dbghelp)

# 生成用于VS的pdb文件
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")       # 设置了Release构建类型下C++编译器的选项，使用 /Zi 选项表示生成包含调试信息的编译器输出文件
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")           # 设置了Release构建类型下C编译器的选项，同样使用 /Zi 选项来生成包含调试信息的编译器输出文件
#  这一行设置了Release构建类型下的链接选项。它为共享库（Shared Library）设置链接器选项，包括 /DEBUG 以生成调试信息，以及 /OPT:REF 和 /OPT:ICF 用于优化（去除未引用的代码和合并相同的代码）
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE
    "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")   # 设置了一个包含目录列表 INCDIRS，包括当前目录 (.)、lib 目录以及 CMAKE_BINARY_DIR 目录（编译输出目录）

set(INCDIRS . lib ${CMAKE_BINARY_DIR})
include_directories(${INCDIRS})                                     # 将 INCDIRS 中定义的目录列表添加到项目的包含路径中，以便编译器能够找到所需的头文件

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11")                     # 启用C11和C20标准
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")              # 输出一些状态信息
message(STATUS "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")

# Debug模式下定义DEBUG宏，Release模式下定义DNDEBUG宏
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

add_subdirectory(src)           # 添加子目录到项目中

if(BUILD_BURIED_EXAMPLES)       # 打开编译实例代码程序时，就将添加一个子目录
    add_subdirectory(examples)
endif()

if(BUILD_BURIED_TEST)
    # 指定头文件的搜索路径，会优先去这个目录下去寻找
    include_directories(
        third_party/googletest-1.14.0/googletest
        third_party/googletest-1.14.0/googletest/include   
    )
    add_subdirectory(third_party/googletest-1.14.0) # 添加三方库子模块
    add_subdirectory(tests)
endif()
