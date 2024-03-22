// ���ڲ���Beast����http����
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

// ִ��HTTP��POST���󣬲��ҽ�����Ӧ���
// ������������Ϊ HttpPost������������������Ϊ DISABLED����ʾ����ǰ������
TEST(BuriedHttpTest, HttpHost)
{
    try
    {
        auto const host = "localhost";
        auto const target = "/buried";
        int version = 11;

        // ���� I/O ����Ҫ io_context
        net::io_context ioc;

        // ��Щ����ִ�����ǵ� I/O, ���Ƿֱ����ڽ�����������ִ�� TCP ͨ��
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc); 
        // ����һ�����ڽ����������Ͷ˿ںŵĶ���
        boost::asio::ip::tcp::resolver::query query(host, "5678");
        // ������ѯ����
        auto const results = resolver.resolve(query);

        // ���ݲ�ѯ���� IP ��ַ�Ͷ˿ڽ�������
        stream.connect(results);

        // ����һ�� HTTP POST ������Ϣ
        // ����������ķ�����Ŀ��·����HTTP �汾�����������û�������������ͣ�������������������
        http::request<http::string_body> req{http::verb::post, target, version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/json");
        req.body() = "{\"hello\":\"world\"}";
        req.prepare_payload();

        // ��Զ���������� HTTP ����
        http::write(stream, req);

        // �û��������ڶ�ȡ��Ӧ����
        beast::flat_buffer buffer;
        
        // ����һ��������������Ӧ
        http::response<http::dynamic_body> res;

        // ��ȡHTTP����Ӧ
        http::read(stream, buffer, res);
        std::string bdy = boost::beast::buffers_to_string(res.body().data());
        std::cout << "bdy " << bdy << std::endl;

        // ����Ϣд�ɱ�׼��ʽ
        std::cout << "res " << res << std::endl;
        std::cout << "res code " << res.result_int() << std::endl;

        // ���Źر�socket
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        // not_connectedʱ�з��������Բ��ر���
        if(ec && ec != beast::errc::not_connected)
        {
            throw beast::system_error{ec};
        }
        // ������ǵ���������Ӿͻᱻ���ŵعر�
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
