#include "UDPServer.h"

#include <unistd.h>
#include <iostream>
#include <cstring>
#include <utility>

UDPServer::UDPServer(uint16_t port_listen)
    :socket(io_service, udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port_listen))
{
}

UDPServer::~UDPServer()
{
    disconnect();
}

void UDPServer::connect()
{
    thread = std::thread(&UDPServer::listen, this);
    connectState = true;
}

void UDPServer::disconnect()
{
    if (thread.joinable())
        thread.join();
}

void UDPServer::listen()
{
    boost::array<uint8_t, 1024> buffer{0};
    udp::endpoint remote_endpoint;

    while(connectState)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        ssize_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);

        if (len > 0)
        {
            if (onData)
                onData(buffer.data(), static_cast<size_t>(len));
        }
    }
}
