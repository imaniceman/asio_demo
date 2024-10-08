#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: " << argv[0] << " <host>" << '\n';
            return 1;
        }
        boost::asio::io_context ioContext;
        tcp::resolver resolver(ioContext);
        // Resolve the host name into an IP address that can be used to connect to the server
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");

        // Create and connect a socket
        tcp::socket socket(ioContext);
        boost::asio::connect(socket, endpoints);

        for (;;)
        {
            std::array<char, 128> buf;
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            if (error == boost::asio::error::eof)
            {
                break;
            }
            else if (error)
            {
                throw boost::system::system_error(error);
            }
            std::cout.write(buf.data(), len);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    return 0;
}
