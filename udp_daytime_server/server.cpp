#include <boost/asio.hpp>
#include<iostream>

using boost::asio::ip::udp;

inline std::string makeDaytimeString()
{
    const time_t now = time(nullptr);
    std::array<char, 26> buf;
    if (const auto err = ctime_s(buf.data(), buf.size(), &now); err != 0)
    {
        return "Error";
    }
    return {buf.data()};
}

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_context ioContext;
        udp::socket socket(ioContext, udp::endpoint(udp::v4(), 13));
        for (;;)
        {
            std::array<char, 1> receiveBuf;
            udp::endpoint remoteEndpoint;
            socket.receive_from(boost::asio::buffer(receiveBuf), remoteEndpoint);
            std::string message = makeDaytimeString();
            boost::system::error_code ignoredError;
            socket.send_to(boost::asio::buffer(message),
                           remoteEndpoint, 0, ignoredError);

        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
