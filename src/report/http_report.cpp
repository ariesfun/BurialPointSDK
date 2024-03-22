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
    // 创建了一个 net::io_context 对象 ioc，它是 Asio 库中用于处理 I/O 操作的核心对象
    static boost::asio::io_context ioc;

    HttpReporter::HttpReporter(std::shared_ptr<spdlog::logger> logger) : logger_(logger)
    {
    }

    bool HttpReporter::Report()
    {
        try
        {
            int version = 11;

            // 这些对象执行我们的 I/O, 它们分别用于解析主机名和执行 TCP 通信
            tcp::resolver resolver(ioc);
            beast::tcp_stream stream(ioc); 

            // 构造一个用于解析主机名和端口号的对象
            boost::asio::ip::tcp::resolver::query query(host_, port_);
            // 解析查询域名
            auto const results = resolver.resolve(query);

            // 根据查询到的 IP 地址和端口进行连接
            stream.connect(results);

            // 设置一个 HTTP POST 请求信息
            // 设置了请求的方法、目标路径、HTTP 版本、主机名、用户代理和内容类型，并设置了请求体内容
            http::request<http::string_body> req{http::verb::post, topic_, version};
            req.set(http::field::host, host_);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            req.set(http::field::content_type, "application/json");
            req.body() = body_;
            req.prepare_payload();

            // 向远程主机发送 HTTP 请求
            http::write(stream, req);

            // 该缓冲区用于读取响应内容
            beast::flat_buffer buffer;
            
            // 声明一个容器来保存响应
            http::response<http::dynamic_body> res;

            // 读取HTTP的响应
            http::read(stream, buffer, res);

            // 优雅关闭socket
            beast::error_code ec;
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);

            // not_connected时有发生，所以不必报告
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
            // 如果我们到了这里，连接就会被优雅地关闭
        }
        catch(const std::exception& e)
        {
            SPDLOG_LOGGER_ERROR(logger_, "report error" + std::string(e.what()));
            return false;
        }
        return true;
    }
} // namespace buried
