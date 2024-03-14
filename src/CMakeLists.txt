include_guard(GLOBAL)           # 在全局范围内防止头文件被重复包含

set(DB_SRCS module/sqlite/sqlite3.c database/database.cpp)

set(BURIED_SRCS                 # 设置变量，埋点相关的源文件
    ${DB_SRCS}
    buried.cpp
    buried_core.cpp)

set(MODULE_NAME "Buried")       # 设置变量，用作模块的名称
message(STATUS "libs ${LIBS}")  # 这个命令会输出一条状态信息，显示变量 ${LIBS} 的值

include_directories(. module/spdlog-1.12.0/include)
add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)   # 定义指定宏，以防导致Custom Formatting功能异常
add_subdirectory(module/spdlog-1.12.0)

if(BUILD_BURIED_STATIC_LIBS)    # 是否构建静态库版本
    add_library(${MODULE_NAME}_static STATIC ${BURIED_SRCS}) 
    target_link_libraries(${MODULE_NAME}_static PRIVATE ${LIBS} spdlog) 
endif()

if(BUILD_BURIED_SHARED_LIBS)    # 是否构建动态库版本
    add_library(${MODULE_NAME}_shared SHARED ${BURIED_SRCS})       # 用于添加一个共享库目标
    target_link_libraries(${MODULE_NAME}_shared PRIVATE ${LIBS} spdlog)   # 用于指定链接目标（所有要链接的依赖库列表）
endif()
