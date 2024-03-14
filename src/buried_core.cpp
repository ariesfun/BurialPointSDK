#include "buried_core.h"

#include "spdlog/sinks/basic_file_sink.h"   // 将日志消息写入到文件中（提供文件输出的功能）
#include "spdlog/sinks/stdout_color_sinks.h"// 将日志消息写入到控制台，并可以根据日志级别使用不同的颜色进行区分
#include "spdlog/spdlog.h"                  // 括了各种日志记录器、格式化器、日志级别等的定义，以及一些常用的配置和初始化函数

Buried::Buried(const std::string& work_dir) 
{
    InitWorkPath_(work_dir); // 初始化工作路径
    InitLogger_();           // 初始化日志配置

    SPDLOG_LOGGER_INFO(Logger(), "Buried init success!");
    SPDLOG_LOGGER_DEBUG(Logger(), "Some debug message");
}

Buried::~Buried() {}

BuriedResult Buried::Start(const Config& config)
{
    return BuriedResult::kBuriedOk;
}

BuriedResult Buried::Report(const char* title, const char* report_data, uint32_t priority)
{
    return BuriedResult::kBuriedOk;
}

void Buried::InitWorkPath_(const std::string& work_dir)
{
    std::filesystem::path _work_dir(work_dir);
    if(!std::filesystem::exists(_work_dir))
    {
        std::filesystem::create_directories(_work_dir);
    }
    work_path_ = _work_dir / "buried"; // 将_work_dir更新为拼接后的新路径

    if(!std::filesystem::exists(work_path_))
    {
        std::filesystem::create_directories(work_path_);
    }
}

void Buried::InitLogger_()
{
    // 用于将日志写入到控制台
    auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();

    std::filesystem::path _log_dir(work_path_);
    _log_dir /= "buried.log";

    // 用于将日志写入到文件
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(_log_dir.string(), true);

    logger_ = std::shared_ptr<spdlog::logger>(new spdlog::logger("buried_sink", {console_sink, file_sink}));

    // 自定义日志的格式
    logger_->set_pattern("[%c] [%s:%#] [%l] %v");

    logger_->set_level(spdlog::level::trace);

    // 通过传入不同的Logger对象，可以做到多实例间的真正隔离
    // SPDLOG_LOGGER_TRACE(logger_, "Some trace message with param: {}", 42);
    // SPDLOG_LOGGER_DEBUG(logger_, "Some debug message");
}

std::shared_ptr<spdlog::logger> Buried::Logger()
{
    return logger_; // 将实例化的指针返回，让其他文件访问到
}