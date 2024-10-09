#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "../includes/common.hpp"
using boost::asio::ip::tcp;


class TcpConnection
    : public std::enable_shared_from_this<TcpConnection>
{
public:
    using Pointer = std::shared_ptr<TcpConnection>;

    static Pointer create(boost::asio::io_context& ioContext)
    {
        // return std::make_shared<TcpConnection>(ioContext);
        return Pointer(new TcpConnection(ioContext));
    }

    tcp::socket& socket()
    {
        return m_socket;
    }

    void start()
    {
        m_message = makeDaytimeString();

        auto self(shared_from_this());
        boost::asio::async_write(m_socket,
                                 boost::asio::buffer(m_message),
                                 [self](const boost::system::error_code& error,
                                        size_t bytes_transferred)
                                 {
                                 }
        );
    }

private:
    tcp::socket m_socket;
    std::string m_message;

    explicit TcpConnection(boost::asio::io_context& ioContext)
        : m_socket(ioContext)
    {
    }
};
