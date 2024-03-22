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
    // ��װһ�׺��õ�http�ӿ�
    class HttpReporter {
    public:
        // ����std::shared_ptr<spdlog::logger>���͵Ĳ��������ڼ�¼��־
        explicit HttpReporter(std::shared_ptr<spdlog::logger> logger); 

    public:
        HttpReporter& Host(const std::string& host)     // ����������
        {
            host_ = host;
            return *this;
        }

        HttpReporter& Topic(const std::string& topic)   // ���þ���·��
        {
            topic_ = topic;
            return *this;
        }

        HttpReporter& Port(const std::string& port)     // ���ö˿ں�
        {
            port_ = port;
            return *this;
        }

        HttpReporter& Body(const std::string& body)     // �������������
        {
            body_ = body;
            return *this;
        }

        bool Report(); // ʹ��Boost��Beast�ⷢ��HTTP���󣬲�����boolֵ

    private:
        std::string host_;
        std::string topic_;
        std::string port_;
        std::string body_;

        std::shared_ptr<spdlog::logger> logger_;

    };
} // namespace buried
