#include <cstdio>
#include <boost/asio.hpp>
using boost::asio::awaitable;
using boost::asio::use_awaitable;
using boost::asio::ip::tcp;
using boost::asio::co_spawn;
using boost::asio::detached;

// namespace this_coro = boost::asio::this_coro;

awaitable<void> echo(tcp::socket socket)
{
    try
    {
        char data[1024];
        for(;;)
        {
            std::size_t n = co_await socket.async_read_some(boost::asio::buffer(data), use_awaitable);
            co_await boost::asio::async_write(socket, boost::asio::buffer(data, n), use_awaitable);
        }
    }
    catch (std::exception&e)
    {
        std::printf("echo Exception:%s\n", e.what());
    }
}
awaitable<void> listener(){
    auto executor = co_await boost::asio::this_coro::executor;
    tcp::acceptor acceptor(executor, {tcp::v4(), 55555});
    for(;;)
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
