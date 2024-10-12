#include <cstdio>
#include <boost/asio.hpp>
using boost::asio::awaitable;
using boost::asio::use_awaitable;
using boost::asio::ip::tcp;
using boost::asio::co_spawn;
using boost::asio::detached;

// namespace this_coro = boost::asio::this_coro;
#if defined(BOOST_ASIO_ENABLE_HANDLER_TRACKING)
#define use_awaitable \
    boost::asio::use_awaitable_t(__FILE__,__LINE__,__PRETTY_FUNCTION__)
#endif

awaitable<void> echoOnce(tcp::socket& socket)
{
    char data[1024];
    std::size_t n = co_await socket.async_read_some(boost::asio::buffer(data), use_awaitable);
    co_await boost::asio::async_write(socket, boost::asio::buffer(data, n), use_awaitable);
}

awaitable<void> echo(tcp::socket socket)
{
    try
    {
        for (;;)
        {
            co_await echoOnce(socket);
        }
    }
    catch (const boost::system::system_error& e)
    {
        if (e.code() == boost::asio::error::eof)
        {
            std::printf("Client close connection\n");
        }
        else
        {
            std::printf("echo Exception:%s\n", e.what());
        }
    }
}

awaitable<void> listener()
{
    auto executor = co_await boost::asio::this_coro::executor;
    tcp::acceptor acceptor(executor, {tcp::v4(), 55555});
    for (;;)
    {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        co_spawn(executor, echo(std::move(socket)), detached);
    }
}

int main()
{
    try
    {
        boost::asio::io_context ioContext(1);
        boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto)
        {
            ioContext.stop();
        });
        co_spawn(ioContext, listener(), detached);
        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::printf("Exception: %s\n", e.what());
    }
}
