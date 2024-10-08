#include <array>
#include <ctime>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

std::string makeDaytimeString()
{
    time_t now = time(nullptr);
    std::array<char, 128> buf;
    if (auto err = ctime_s(buf.data(), buf.size(), &now); err != 0)
    {
        return "Error";
    }
    return buf.data();
}

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_context ioContext;
        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 13));
        for(;;)
        {
            tcp::socket socket(ioContext);
            acceptor.accept(socket);
            std::string message = makeDaytimeString();
            boost::system::error_code ignoredError;
            boost::asio::write(socket, boost::asio::buffer(message), ignoredError);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
