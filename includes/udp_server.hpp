#pragma once
#include <boost/asio.hpp>
#include "common.hpp"
using boost::asio::ip::udp;


class UdpServer
{
public :
    explicit UdpServer(boost::asio::io_context& ioContext)
        : m_ioContext(ioContext),
          m_socket(ioContext, udp::endpoint(udp::v4(), 13))
    {
        startReceive();
    }

private:
    boost::asio::io_context& m_ioContext;
    udp::socket m_socket;
    udp::endpoint m_remoteEndpoint;
    std::array<char, 1> m_receiveBuf;

    void startReceive()
    {
        // m_socket.receive_from(boost::asio::buffer(receiveBuf), remoteEndpoint);
        // ¸Ä³ÉÒì²½
        m_socket.async_receive_from(boost::asio::buffer(m_receiveBuf),
                                    m_remoteEndpoint,
                                    [this](const boost::system::error_code& error, const std::size_t bytesTransferred)
                                    {
                                        handleReceive(error, bytesTransferred);
                                    }
        );
    }


    void handleReceive(const boost::system::error_code& error, std::size_t /*bytesTransferred*/)
    {
        if (!error)
        {
            auto message = std::make_shared<std::string>(makeDaytimeString());
            m_socket.async_send_to(boost::asio::buffer(*message), m_remoteEndpoint,
                                   [this,message](boost::system::error_code, size_t)
                                   {
                                   });
            startReceive();
        }
    }
};

