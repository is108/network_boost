#pragma once
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>

#include <functional>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

class TCPClient
{
public:
    explicit TCPClient(std::string ip, uint16_t port);
    ~TCPClient();

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
    std::function<void(const uint8_t* data, size_t size)> onData;
    std::function<void()> onConnected;

private:
    void read();
private:
    std::atomic_bool connectState = {false};
    std::string m_ip;
    uint16_t m_port;
    boost::asio::io_service io_service;
    boost::scoped_ptr<boost::asio::ip::tcp::socket> socket;
    boost::system::error_code error;
    tcp::endpoint endpoint;
    std::thread thread;
};


