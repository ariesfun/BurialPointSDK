#include "include/buried.h"
#include "buried_core.h"
#include <iostream>

#include <spdlog/spdlog.h>

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

extern "C"
{
    int BuriedTest()
    {
        std::cout << "Buried Test." << std::endl;

        // 设置日志级别为DEBUG
        spdlog::set_level(spdlog::level::debug);
        // 输出不同级别的日志
        spdlog::debug("This is a debug message");
        spdlog::info("This is an info message");
        spdlog::warn("This is a warning message");
        spdlog::error("This is an error message");

        // JsonCpp功能测试
        // 01.序列化一个json数据
        Json::Value root;  // 定义子对象
        root["id"] = 1001; // 添加数据
        root["data"] = "hello world";
        std::string request = root.toStyledString();             // 转为字符串，输出到控制台
        std::cout << "request is " << request << std::endl;     
        // 解析json字符串
        Json::Value root2;
        Json::Reader reader;
        reader.parse(request, root2);
        std::cout << "msg id is " << root2["id"] << " msg is " << root2["data"] << std::endl;
        
        return 1;
    }

    Buried* Buried_Create(const char* work_dir)
    {
        if(!work_dir)
        {
            return nullptr;
        }
        return new Buried(work_dir);
    }

    void Buried_Destory(Buried* buried)
    {
        if(buried)
        {
            delete buried;
        }
    }

    int32_t Buried_Start(Buried* buried, BuriedConfig* config)
    {
        if(!buried || !config)
        {
            return BuriedResult::kBuriedInvalidParam;
        }

        Buried::Config buried_config;
        if(config->host)
        {
            buried_config.host = config->host;
        }
        if(config->topic)
        {
            buried_config.topic = config->topic;
        }
        if(config->user_id)
        {
            buried_config.user_id = config->user_id;
        }
        if(config->app_version)
        {
            buried_config.app_version = config->app_version;
        }  
        if(config->app_name)
        {
            buried_config.custom_data = config->custom_data;
        }
        if(config->custom_data)
        {
            buried_config.custom_data = config->custom_data;
        }
        return buried->Start(buried_config);     // 内部转为C++方式的调用 
    }

    int32_t Buried_Report(Buried* buried, const char* title, const char* report_data, uint32_t priority)
    {
        if(!buried || !title || !report_data)
        {
            return BuriedResult::kBuriedInvalidParam;
        }
        return buried->Report(title, report_data, priority);
    }
}
