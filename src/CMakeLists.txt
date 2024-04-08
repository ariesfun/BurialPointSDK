include_guard(GLOBAL)           # 在全局范围内防止头文件被重复包含

include_directories(. module module/spdlog-1.12.0/include moudle/boost module/jsoncpp-1.9.5/include module/mbedtls-3.5.0/include)
add_definitions(-D_WIN32_WINNT=0x0601)
add_definitions(-DBOOST_JSON_NO_LIB)   # 告诉编译器不要链接Boost.JSON库
add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)   # 定义指定宏，以防导致Custom Formatting功能异常
add_subdirectory(module/spdlog-1.12.0) # 会编译各自子目录的源码
add_subdirectory(module/jsoncpp-1.9.5)
add_subdirectory(module/mbedtls-3.5.0/library)

set(DB_SRCS module/sqlite/sqlite3.c database/database.cpp)
set(REPORT_SRCS report/http_report.cpp)
set(CRYPT_SRCS crypt/crypt.cpp)
set(CONTEXT_SRCS context/context.cpp)

set(BURIED_SRCS                 # 设置变量，埋点相关的源文件
    ${DB_SRCS}
    ${REPORT_SRCS}
    ${CRYPT_SRCS}
    ${CONTEXT_SRCS}
    buried.cpp
    buried_core.cpp)

set(MODULE_NAME "Buried")       # 设置变量，用作模块的名称
message(STATUS "libs ${LIBS}")  # 这个命令会输出一条状态信息，显示变量 ${LIBS} 的值


if(BUILD_BURIED_STATIC_LIBS)    # 是否构建静态库版本
    add_library(${MODULE_NAME}_static STATIC ${BURIED_SRCS}) 
    target_link_libraries(${MODULE_NAME}_static PRIVATE ${LIBS} spdlog jsoncpp_static mbedtls_static) 
endif()

if(BUILD_BURIED_SHARED_LIBS)    # 是否构建动态库版本
    add_library(${MODULE_NAME}_shared SHARED ${BURIED_SRCS})       # 用于添加一个共享库目标
    target_link_libraries(${MODULE_NAME}_shared PRIVATE ${LIBS} spdlog jsoncpp_static mbedtls_static)   # 用于指定链接目标（所有要链接的依赖库列表）
endif()
