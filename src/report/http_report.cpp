#include "http_report.h"

#include "src/module/boost/asio/connect.hpp"
#include "src/module/boost/asio/io_context.hpp"
#include "src/module/boost/asio/ip/tcp.hpp"
#include "src/module/boost/beast/core.hpp"
#include "src/module/boost/beast/http.hpp"
#include "src/module/boost/beast/version.hpp"
#include "spdlog/spdlog.h"

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

namespace buried
{
    // ������һ�� net::io_context ���� ioc������ Asio �������ڴ��� I/O �����ĺ��Ķ���
    static boost::asio::io_context ioc;

    HttpReporter::HttpReporter(std::shared_ptr<spdlog::logger> logger) : logger_(logger)
    {
    }

    bool HttpReporter::Report()
    {
        try
        {
            int version = 11;

            // ��Щ����ִ�����ǵ� I/O, ���Ƿֱ����ڽ�����������ִ�� TCP ͨ��
            tcp::resolver resolver(ioc);
            beast::tcp_stream stream(ioc); 

            // ����һ�����ڽ����������Ͷ˿ںŵĶ���
            boost::asio::ip::tcp::resolver::query query(host_, port_);
            // ������ѯ����
            auto const results = resolver.resolve(query);

            // ���ݲ�ѯ���� IP ��ַ�Ͷ˿ڽ�������
            stream.connect(results);

            // ����һ�� HTTP POST ������Ϣ
            // ����������ķ�����Ŀ��·����HTTP �汾�����������û�������������ͣ�������������������
            http::request<http::string_body> req{http::verb::post, topic_, version};
            req.set(http::field::host, host_);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            req.set(http::field::content_type, "application/json");
            req.body() = body_;
            req.prepare_payload();

            // ��Զ���������� HTTP ����
            http::write(stream, req);

            // �û��������ڶ�ȡ��Ӧ����
            beast::flat_buffer buffer;
            
            // ����һ��������������Ӧ
            http::response<http::dynamic_body> res;

            // ��ȡHTTP����Ӧ
            http::read(stream, buffer, res);

            // ���Źر�socket
            beast::error_code ec;
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);

            // not_connectedʱ�з��������Բ��ر���
            if(ec && ec != beast::errc::not_connected)
            {
                throw beast::system_error{ec};
            }

            auto res_status = res.result();
            if(res_status != http::status::ok)
            {
                SPDLOG_LOGGER_ERROR(logger_, "report error " + std::to_string(res.result_int()));
                return false;
            }

            std::string res_body = boost::beast::buffers_to_string(res.body().data());
            SPDLOG_LOGGER_TRACE(logger_, "report success" + res_body);
            // ������ǵ���������Ӿͻᱻ���ŵعر�
        }
        catch(const std::exception& e)
        {
            SPDLOG_LOGGER_ERROR(logger_, "report error" + std::string(e.what()));
            return false;
        }
        return true;
    }
} // namespace buried
