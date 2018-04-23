#pragma once

#include "server_base.h"

namespace csx {

typedef boost::asio::ip::tcp::socket HTTP;
template<>
class Server<HTTP> : public ServerBase<HTTP> {
public:
    Server(unsigned short port, size_t num_threads = 1)
        : ServerBase<HTTP>::ServerBase(port, num_threads) {}
private:
    // ʵ��accept����
    void accept() {
        // Ϊ��ǰ���Ӵ���һ���µ�socket
        // shared_ptr���ڴ���һ����ʱ��������������
        // socket �ᱻ�Ƶ�Ϊstd::shared_ptr<HTTP>����
        auto socket = std::make_shared<HTTP>(m_io_service);

        acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& ec){
            // ��������������һ������
            accept();

            //
            if (!ec) {
                process_request_and_response(socket);
            }
        });
    }
};
}
