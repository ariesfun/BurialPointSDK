#pragma once

#include <string>
#include <stdint.h>

#include "buried_common.h"
#include "include/buried.h"

// ��㹦�ܵľ��嶨��
// ���ڶ�C�ӿڵ��Ž�ת�����ڲ���C++ָ��
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
