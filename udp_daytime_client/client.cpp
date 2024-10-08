#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;
int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: client <host>" << '\n';
            return 1;
        }
        boost::asio::io_context ioContext;
        udp::resolver resolver(ioContext);
        udp::endpoint receiverEndpoint =
            *resolver.resolve(udp::v4(), argv[1], "daytime").begin();
        udp::socket socket(ioContext);
        socket.open(udp::v4());

        std::array<char, 1> sendBuf = {{0}};
        socket.send_to(boost::asio::buffer(sendBuf), receiverEndpoint);

        std::array<char, 128> receiveBuf;
        udp::endpoint senderEndpoint;
        size_t len = socket.receive_from(
            boost::asio::buffer(receiveBuf), senderEndpoint);
        std::cout.write(receiveBuf.data(), len);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
