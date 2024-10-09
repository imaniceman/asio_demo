#include<iostream>
#include <boost/asio.hpp>
#include "../includes/udp_server.hpp"
#include "../includes/tcp_server.hpp"

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_context ioContext;
        UdpServer server1(ioContext);
        TcpServer server2(ioContext);
        ioContext.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
