#pragma once
#include <stdint.h> // 明确指定整数类型的大小

#define BURIED_EXPORT __declspec(dllexport) // 用于指示编译器将函数或变量导出到动态链接库（DLL）中

// 使用 extern "C" 声明，可以确保编译器按照 C 的调用约定而不是 C++ 的调用约定来处理这些函数
// 从而使得它们可以在不同的编程语言之间进行交互
extern "C"
{
    BURIED_EXPORT int BuriedTest(); 

    typedef struct Buried Buried;

    // 定义对外暴露的C接口,便于与其他语言进行交互
    struct BuriedConfig
    {
        const char* host;
        const char* topic;
        const char* user_id;
        const char* app_version;
        const char* app_name;
        const char* custom_data;
    };

    BURIED_EXPORT Buried* Buried_Create(const char* work_dir);

    BURIED_EXPORT void Buried_Destory(Buried* buried);

    BURIED_EXPORT int32_t Buried_Start(Buried* buried, BuriedConfig* config);

    BURIED_EXPORT int32_t Buried_Report(Buried* buried, const char* title, const char* report_data, uint32_t priority);

}
