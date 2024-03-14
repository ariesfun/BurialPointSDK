#pragma once

#include <string>
#include <stdint.h>
#include <memory>
#include <filesystem>                       // C++17标准，用于操作文件系统的库

#include "buried_common.h"
#include "include/buried.h"

namespace spdlog
{
    class logger; // 用于日志库中类的前向声明
} // namespace spdlog


// 埋点功能的具体定义
// 用于对C接口的桥接转换，内部是C++指针
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
    void InitWorkPath_(const std::string& work_dir); // 初始化工作路径
    void InitLogger_();

public:
    std::shared_ptr<spdlog::logger> Logger(); // 创建公有的智能指针Logger()

private:
    std::shared_ptr<spdlog::logger> logger_; // 创建了一个名为 logger_ 的智能指针，指向 spdlog::logger 类的实例
                                             // std::shared_ptr 智能指针类型，它允许多个指针共享同一个对象，并在所有指针都释放对对象的引用后自动销毁对象
    
    std::filesystem::path work_path_;        // 全局的工作路径
};
