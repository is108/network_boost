#pragma once

#include <atomic>
#include <cstdint>
#include <thread>
#include <string>
#include <functional>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class UDPServer
{
public:
    explicit UDPServer(uint16_t port_listen);
    ~UDPServer();

    void connect();
    void disconnect();

    std::function<void(const uint8_t* data, size_t size)> onData;
private:
    void listen();

private:
    boost::asio::io_service io_service;
    udp::socket socket;
    std::atomic_bool connectState = {false};
    std::thread thread;
};


