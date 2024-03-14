#pragma once

#include <string>
#include <stdint.h>
#include <memory>

#include "buried_common.h"
#include "include/buried.h"

namespace spdlog
{
    class logger; // ������־�������ǰ������
} // namespace spdlog


// ��㹦�ܵľ��嶨��
// ���ڶ�C�ӿڵ��Ž�ת�����ڲ���C++ָ��
struct Buried {
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

private:
    std::shared_ptr<spdlog::logger> logger_; // ������һ����Ϊ logger_ ������ָ�룬ָ�� spdlog::logger ���ʵ��
                                             // std::shared_ptr ����ָ�����ͣ����������ָ�빲��ͬһ�����󣬲�������ָ�붼�ͷŶԶ�������ú��Զ����ٶ���
};