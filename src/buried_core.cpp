#include "buried_core.h"

#include <filesystem>                       // C++17��׼�����ڲ����ļ�ϵͳ�Ŀ�
#include "spdlog/sinks/basic_file_sink.h"   // ����־��Ϣд�뵽�ļ��У��ṩ�ļ�����Ĺ��ܣ�
#include "spdlog/sinks/stdout_color_sinks.h"// ����־��Ϣд�뵽����̨�������Ը�����־����ʹ�ò�ͬ����ɫ��������
#include "spdlog/spdlog.h"                  // ���˸�����־��¼������ʽ��������־����ȵĶ��壬�Լ�һЩ���õ����úͳ�ʼ������

Buried::Buried(std::string work_dir) 
{
    // ���ڽ���־д�뵽����̨
    auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();

    std::filesystem::path _work_dir(work_dir);
    if(!std::filesystem::exists(_work_dir))
    {
        std::filesystem::create_directories(_work_dir);
    }
    _work_dir /= "buried"; // ��_work_dir����Ϊƴ�Ӻ����·��

    if(!std::filesystem::exists(_work_dir))
    {
        std::filesystem::create_directories(_work_dir);
    }

    std::filesystem::path _log_dir(_work_dir);
    _log_dir /= "buried.log";

    // ���ڽ���־д�뵽�ļ�
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(_log_dir.string(), true);

    logger_ = std::shared_ptr<spdlog::logger>(new spdlog::logger("buried_sink", {console_sink, file_sink}));

    // �Զ�����־�ĸ�ʽ
    logger_->set_pattern("[%c] [%s:%#] [%l] %v");

    logger_->set_level(spdlog::level::trace);

    // ͨ�����벻ͬ��Logger���󣬿���������ʵ�������������
    SPDLOG_LOGGER_TRACE(logger_, "Some trace message with param: {}", 42);
    SPDLOG_LOGGER_DEBUG(logger_, "Some debug message");

}

Buried::~Buried() {}

BuriedResult Buried::Start(const Config& config)
{
    return BuriedResult::kBuriedOk;
}

BuriedResult Buried::Report(const char* report_data, uint32_t priority)
{
    return BuriedResult::kBuriedOk;
}