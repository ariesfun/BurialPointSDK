#pragma once

#include <string>
#include <stdint.h>

#include "buried_common.h"
#include "include/buried.h"

// 埋点功能的具体定义
// 用于对C接口的桥接转换，内部是C++指针
class Buried {
public:
    struct Config
    {
        const char* host;
        const char* topic;
        const char* user_id;
        const char* app_version;
        const char* custom_data;
    };

public:
    Buried(std::string work_dir);
    ~Buried();
    BuriedResult Start(const Config& config);
    BuriedResult Report(const char* report_data, uint32_t priority);

};
