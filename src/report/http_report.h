#pragma once

#include <stdint.h>
#include <memory>
#include <string>

namespace spdlog 
{
    class logger;
}

namespace buried
{
    // 封装一套好用的http接口
    class HttpReporter {
    public:
        // 接收std::shared_ptr<spdlog::logger>类型的参数，用于记录日志
        explicit HttpReporter(std::shared_ptr<spdlog::logger> logger); 

    public:
        HttpReporter& Host(const std::string& host)     // 设置主机名
        {
            host_ = host;
            return *this;
        }

        HttpReporter& Topic(const std::string& topic)   // 设置具体路径
        {
            topic_ = topic;
            return *this;
        }

        HttpReporter& Port(const std::string& port)     // 设置端口号
        {
            port_ = port;
            return *this;
        }

        HttpReporter& Body(const std::string& body)     // 设置请求的主体
        {
            body_ = body;
            return *this;
        }

        bool Report(); // 使用Boost的Beast库发起HTTP请求，并返回bool值

    private:
        std::string host_;
        std::string topic_;
        std::string port_;
        std::string body_;

        std::shared_ptr<spdlog::logger> logger_;

    };
} // namespace buried
