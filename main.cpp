#include "server.h"

#include <iostream>

#include <asio/io_context.hpp>

int main(int argc, char *argv[])
{
    try {
        asio::io_context io_ctx;

        Server srv(io_ctx, 32019);

        io_ctx.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
