#pragma once
#include <boost/asio.hpp>

#include "tcp_connection.hpp"

using boost::asio::ip::tcp;

class TcpServer
{
public:
    explicit TcpServer(boost::asio::io_context& ioContext)
        : m_ioContext(ioContext),
          m_acceptor(ioContext, tcp::endpoint(tcp::v4(), 13))
    {
        startAccept();
    }

private:
    boost::asio::io_context& m_ioContext;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    tcp::acceptor m_acceptor;

    void startAccept()
    {
        TcpConnection::Pointer newConnection = TcpConnection::create(m_ioContext);
        m_acceptor.async_accept(newConnection->socket(),
                                [this,newConnection](const boost::system::error_code& error)
                                {
                                    if (!error)
                                    {
                                        newConnection->start();
                                    }
                                    startAccept();
                                }
        );
    }
};
