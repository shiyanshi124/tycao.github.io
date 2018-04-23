#pragma once
#include <unordered_map>    /** for unordered_map<>         */
#include <thread>           /** for std::thread             */
#include <memory>           /** for std::shared_ptr<>       */
#include <iostream>         /** for std::istream            */
#include <functional>       /** for function<>              */
#include <map>              /** for map<>                   */
#include <vector>           /** for vector<>                */
#include <boost/asio.hpp>   /** for boost::asio<>           */
#include <regex>            /** for std::regex std::smatch  */
#include <string>           /** for std::stoull             */

namespace csx {

// ��Ҫ����һ���ṹ��Request�����ڽ������������󷽷�������·����HTTP�汾����Ϣ
struct Request {
    // ���󷽷���POST,GET; ����·���� HTTP�汾
    std::string method, path, http_version;

    // ��contentʹ������ָ��������ü���
    std::shared_ptr<std::istream> content;

    // hash������k-v�ֵ�
    std::unordered_map<std::string, std::string> header;

    // ��������ʽ����·������
    std::smatch path_match;
};

// �����������Դ����
typedef std::map<std::string, std::unordered_map<std::string,
                            std::function<void(std::ostream&, Request&)>>> resource_type;

// socket_typeΪHTTP����HTTPS
template<typename socket_type>
class ServerBase {
public:
    // ���캯��
    ServerBase(unsigned short port, size_t num_threads = 1);

    // ����������
    void            start();
protected:
    // ��Ҫ��ͬ���͵ķ�����ʵ���������
    virtual void    accept() {}

    // ���������Ӧ��
    void            process_request_and_response(std::shared_ptr<socket_type> socket) const;

    // --��������
    Request         parse_request(std::istream&) const;

    // --Ӧ��
    void            respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const;

public:
    // ���ڷ�����������Դ����ʽ
    resource_type resource;

    // ���ڱ���Ĭ����Դ�Ĵ���ʽ
    resource_type default_resource;
protected:
    // �����ڲ�ʵ�ֶ�������Դ�Ĵ���
    std::vector<resource_type::iterator> all_resource;

    // ����Boost Asio��Ҫ�Ĳ���
    boost::asio::io_service         m_io_service;
    boost::asio::ip::tcp::endpoint  endpoint;
    boost::asio::ip::tcp::acceptor  acceptor;

private:
    size_t num_threads;
    std::vector<std::thread> threads;
};

template<typename socket_type>
class Server : public ServerBase<socket_type> { };
}







// ���캯��
template<typename socket_type>
csx::ServerBase<socket_type>::ServerBase(unsigned short port, size_t num_thread)
    : endpoint(boost::asio::ip::tcp::v4(), port),
      acceptor(m_io_service, endpoint),
      num_threads(num_threads)
{

}

// ����������
template<typename socket_type>
void csx::ServerBase<socket_type>::start() {
    // Ĭ����ԴӦ�÷���vector��ĩβ������Ĭ��Ӧ�� ��
    // Ĭ�ϵ���������Ҳ���ƥ������·��ʱ�����з��ʡ������������
    for (resource_type::iterator iter = resource.begin(); iter != resource.end(); ++iter) {
        all_resource.push_back(iter);
    }
    for (auto iter = default_resource.begin(); iter != default_resource.end(); ++iter) {
        all_resource.push_back(iter);
    }

    // ����socket�����ӷ�ʽ������Ҫ������ʵ��accept()���߼�
    accept();

    // ���num_threads > 1, ��ôִ��m_io_service.run()
    // �����У�num_threads - 1�����̳߳�Ϊ�̳߳�
    for (size_t c = 0; c != threads.size(); c++) {
        threads.emplace_back([this]{
            m_io_service.run();
        });
    }
    // ���߳�
    m_io_service.run();

    // �ȴ������̣߳�����еĻ����͵ȴ���Щ�߳̽���
    for (std::thread& t : threads) {
        t.join();
    }
}

// ���������Ӧ��
template<typename socket_type>
void csx::ServerBase<socket_type>::process_request_and_response(std::shared_ptr<socket_type> socket) const {
    // Ϊasync_read_until() �����µĶ�����
    // shared_ptr���ڴ�����ʱ�����lambda������������
    // auto�ᱻ�Զ��Ƶ�Ϊ std::shared_ptr<boost::asio::streambuf>
    auto read_buffer = std::make_shared<boost::asio::streambuf>();
    /**
     * async_read_until�첽��ȡϵ��, ���������ԭ����, ����һ��streambuf, ��һ���ָ���,asio�����ָ���ʱ����;
     * ����Դ�streambuf�ж�ȡ��Ҫ�����ݡ������� async_read_until �����ɹ���,  streambuf �ڽ綨��֮����ܰ���һЩ����ĵ�����
     * reference link : http://www.cppblog.com/sunicdavy/archive/2012/12/03/195921.html
     */
    boost::asio::async_read_until(*socket, *read_buffer, "\r\n\r\n",
                    [this, socket, read_buffer](const boost::system::error_code& ec, size_t bytes_transferred){
        if (!ec) {
            // ���ԽϺõ�������ֱ�Ӵ�������ȡ��������ǰ read_buffer ��ߵı�ͷ,��ƴ��async_read_until���������
            size_t total = read_buffer->size();

            // ת����istream : ֮��streambuf���������
            std::istream stream(read_buffer.get());

            // auto ���Զ��Ƶ�Ϊ std::make_shared<Request>
            auto request = std::make_shared<Request>();

            // �������ǽ����е�������Ϣ���н�����Ȼ�󱣴���request������
            *request = parse_request(stream);

            // async_read_until()��ȡ�Ķ��������
            size_t num_additional_bytes = total - bytes_transferred;

            // ������㣬ͬ����ȡ
            if (request->header.count("Content-Length") > 0) {
                boost::asio::async_read(*socket, *read_buffer,
                boost::asio::transfer_exactly(std::stoull(request->header["Content-Length"]) - num_additional_bytes),
                [this, socket, read_buffer, request](const boost::system::error_code& ec, size_t bytes_transferred){
                    if (!ec) {
                        // ��ָ����Ϊistream����洢��read_buffer��
                        request->content = std::make_shared<std::istream>(read_buffer.get());
                        respond(socket, request);
                    }
                });
            }else {
                respond(socket, request);
            }
        }
    });
}

// --��������
template<typename socket_type>
csx::Request csx::ServerBase<socket_type>::parse_request(std::istream& stream) const {
    Request request;
    // ʹ��������ʽ������ͷ���н�����ͨ�������������ʽ
    // ���Խ��������󷽷�(GET/POST)������·���Լ� HTTP �汾
    std::regex e("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::smatch sub_match;
    // �����еĵ�һ���У����������󷽷���·����HTTP�汾
    std::string line;
    std::getline(stream, line);
    // line�õ���һ��֮�󣬰����Ļ��з���'CRLF'ȥ����
    line.pop_back();
    if (std::regex_match(line, sub_match, e)) {
        request.method          = sub_match[1];
        request.path            = sub_match[2];
        request.http_version    = sub_match[3];

        // ��������ͷ����������Ϣ
        bool matched;
        e = "^([^:]*): ?(.*)$";
        do {
            // ����ȥ���е���һ������
            getline(stream, line);
            //ȥ��CTRL������ţ�����Ϥ��ͬѧ���ο�https://github.com/tycao/tycao.github.io/blob/master/HTTP_Protocol.md�˽�HTTP�Ľṹ��
            line.pop_back();
            matched = std::regex_match(line, sub_match, e);
            if (matched) {
                request.header[sub_match[1]] = sub_match[2];
            }

        } while (matched == true);
    }
    return request;
}

template<typename socket_type>
void csx::ServerBase<socket_type>::respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const {
    // ������·���ͷ�������ƥ����ң���������Ӧ
    for (auto res_it : all_resource) {
            std::regex e(res_it->first);
            std::smatch sm_res;
            if (std::regex_match(request->path, sm_res, e)) {
                if (res_it->second.count(request->method) > 0) {
                    request->path_match = std::move(sm_res);

                    // auto�ᱻ�Ƶ�Ϊstd::shared_ptr<boost::asio::streambuf>
                    auto write_buffer = std::make_shared<boost::asio::streambuf>();
                    std::ostream response(write_buffer.get());  // ����һ��std::pstream���͵�response����
                    res_it->second[request->method](response, *request);

                    // ��lambda�в���write_buffer��ʹ�䲻����async_write���ǰ������
                    boost::asio::async_write(*socket, *write_buffer,
                         [this, socket, request, write_buffer](const boost::system::error_code& ec, size_t bytes_transferred){
                        // HTTP �����ӣ�HTTP/1.1�����ݹ����
                        if (!ec && std::stof(request->http_version) > 1.05) {
                            process_request_and_response(socket);
                        }
                    });
                    return;
                }
            }
    }
}
