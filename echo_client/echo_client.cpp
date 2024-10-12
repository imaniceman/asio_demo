#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

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
        // 发送一个 echo 请求到 端口 55555
        tcp::resolver resolver(ioContext);
        tcp::resolver::results_type endpoint = resolver.resolve(argv[1], "55555");

        tcp::socket socket(ioContext);
        boost::asio::connect(socket, endpoint);

        const std::string message = "Hello, echo server!";
        auto n = boost::asio::write(socket, boost::asio::buffer(message));

        std::array<char, 128> buf;
        boost::system::error_code error;
        size_t replyLen = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof)
            std::cout << "Connection closed by server." << std::endl;
        else if (error)
            throw boost::system::system_error(error);
        std::cout.write(buf.data(), replyLen);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    return 0;
}
