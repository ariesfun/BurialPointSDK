#pragma once
#include <stdint.h> // 明确指定整数类型的大小

int BuriedTest(); 

typedef struct Buried Buried;

// 定义对外暴露的C接口,便于与其他语言进行交互
struct BuriedConfig
{
    const char* host;
    const char* topic;
    const char* user_id;
    const char* app_version;
    const char* custom_data;
};

Buried* Buried_Create(const char* work_dir);

void Buried_Destory(Buried* buried);

int32_t Buried_Start(Buried* buried, BuriedConfig* config);

int32_t Buried_Report(Buried* buried, const char* report_data, uint32_t priority);
