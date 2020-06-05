#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::udp;

class UDPClient
{
public:
    explicit UDPClient(const std::string &ip, uint16_t port);
    ~UDPClient();

    void connect();
    void disconnect();
    bool send(const uint8_t *data, size_t size);

    std::string ip() const
    {
        return m_ip;
    }
    uint16_t port() const noexcept
    {
        return m_port;
    }

private:

private:
    boost::asio::io_service io_service;
    std::string m_ip;
    udp::socket socket;
    uint16_t m_port;
    udp::endpoint receiver_endpoint;

};


