#pragma once

#include <string>
#include <stdint.h>
#include <memory>
#include <filesystem>                       // C++17��׼�����ڲ����ļ�ϵͳ�Ŀ�

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
        const char* app_name;
        const char* custom_data;
    };

public:
    Buried(const std::string& work_dir);
    ~Buried();
    BuriedResult Start(const Config& config);
    BuriedResult Report(const char* title, const char* report_data, uint32_t priority);

private:
    void InitWorkPath_(const std::string& work_dir); // ��ʼ������·��
    void InitLogger_();

public:
    std::shared_ptr<spdlog::logger> Logger(); // �������е�����ָ��Logger()

private:
    std::shared_ptr<spdlog::logger> logger_; // ������һ����Ϊ logger_ ������ָ�룬ָ�� spdlog::logger ���ʵ��
                                             // std::shared_ptr ����ָ�����ͣ���������ָ�빲��ͬһ�����󣬲�������ָ�붼�ͷŶԶ�������ú��Զ����ٶ���
    
    std::filesystem::path work_path_;        // ȫ�ֵĹ���·��
};
