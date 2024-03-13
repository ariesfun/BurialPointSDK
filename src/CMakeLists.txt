include_guard(GLOBAL)           # 在全局范围内防止头文件被重复包含

set(BURIED_SRCS buried.cpp)     # 设置变量，埋点相关的源文件
set(MODULE_NAME "Buried")       # 设置变量，用作模块的名称
message(STATUS "libs ${LIBS}")  # 这个命令会输出一条状态信息，显示变量 ${LIBS} 的值

if(BUILD_BURIED_SHARED_LIBS)    # 是否构建动态库版本
    add_library(${MODULE_NAME} SHARED ${BURIED_SRCS})       # 用于添加一个共享库目标
    target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS}) # 用于指定链接目标（所有要链接的依赖库列表）
endif()

if(BUILD_BURIED_STATIC_LIBS)    # 是否构建静态库版本
    add_library(${MODULE_NAME}_static STATIC ${BURIED_SRCS}) 
    target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS}) 
endif()
