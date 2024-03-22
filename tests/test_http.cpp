// 用于测试Beast发起http请求
#include <string>
#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"
#include "src/module/boost/asio/connect.hpp"
#include "src/module/boost/asio/ip/tcp.hpp"
#include "src/module/boost/beast/core.hpp"
#include "src/module/boost/beast/http.hpp"
#include "src/module/boost/beast/version.hpp"

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// 执行HTTP的POST请求，并且解析响应结果
// 测试用例名称为 HttpPost，这个测试用例被标记为 DISABLED，表示它当前被禁用
TEST(BuriedHttpTest, HttpHost)
{
    try
    {
        auto const host = "localhost";
        auto const target = "/buried";
        int version = 11;

        // 所有 I/O 都需要 io_context
        net::io_context ioc;

        // 这些对象执行我们的 I/O, 它们分别用于解析主机名和执行 TCP 通信
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc); 
        // 构造一个用于解析主机名和端口号的对象
        boost::asio::ip::tcp::resolver::query query(host, "5678");
        // 解析查询域名
        auto const results = resolver.resolve(query);

        // 根据查询到的 IP 地址和端口进行连接
        stream.connect(results);

        // 设置一个 HTTP POST 请求信息
        // 设置了请求的方法、目标路径、HTTP 版本、主机名、用户代理和内容类型，并设置了请求体内容
        http::request<http::string_body> req{http::verb::post, target, version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/json");
        req.body() = "{\"hello\":\"world\"}";
        req.prepare_payload();

        // 向远程主机发送 HTTP 请求
        http::write(stream, req);

        // 该缓冲区用于读取响应内容
        beast::flat_buffer buffer;
        
        // 声明一个容器来保存响应
        http::response<http::dynamic_body> res;

        // 读取HTTP的响应
        http::read(stream, buffer, res);
        std::string bdy = boost::beast::buffers_to_string(res.body().data());
        std::cout << "bdy " << bdy << std::endl;

        // 将信息写成标准格式
        std::cout << "res " << res << std::endl;
        std::cout << "res code " << res.result_int() << std::endl;

        // 优雅关闭socket
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        // not_connected时有发生，所以不必报告
        if(ec && ec != beast::errc::not_connected)
        {
            throw beast::system_error{ec};
        }
        // 如果我们到了这里，连接就会被优雅地关闭
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
