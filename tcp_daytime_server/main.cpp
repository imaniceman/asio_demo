#include <iostream>
#include <boost/asio.hpp>

#include "tcp_server.hpp"
using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_context ioContext;
        TcpServer server(ioContext);
        ioContext.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
