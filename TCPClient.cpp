#include "TCPClient.h"

#include <iostream>
#include <boost/array.hpp>

using namespace std;

TCPClient::TCPClient(std::string ip, uint16_t port)
    : m_ip(std::move(ip))
    , m_port(port)
    , endpoint( boost::asio::ip::address::from_string(m_ip), m_port )
{
    socket.reset(new boost::asio::ip::tcp::socket(io_service)); 
}

void TCPClient::connect()
{
    socket->connect(tcp::endpoint( boost::asio::ip::address::from_string(m_ip), m_port ), error);
    connectState = true;

    thread = std::thread(&TCPClient::read, this);

    if (error == nullptr) 
        std::cout << "Connect to server [" << m_ip << ':' << m_port << ']' << std::endl;
}

void TCPClient::disconnect()
{
    socket.reset();
    std::cout << "Disconnect to server [" << m_ip << ':' << m_port << ']' << std::endl;

    connectState = false;
    
    if (thread.joinable())
        thread.join();
}

TCPClient::~TCPClient()
{
    disconnect();
}

bool TCPClient::send(const uint8_t *data, size_t size)
{
    if (error || error == boost::asio::error::eof)
    {
        //try reconnect
        socket.reset(new boost::asio::ip::tcp::socket(io_service));
        socket->connect(tcp::endpoint(boost::asio::ip::address::from_string(m_ip), m_port), error);
        
        if (error == nullptr) 
            std::cout << "Connect to server [" << m_ip << ':' << m_port << ']' << std::endl;
        
        return false;
    }
    
    if (error == nullptr)
    {
        boost::asio::write(*socket, boost::asio::buffer(data, size), error);
        
        if (error)
            std::cout << "Disconnect to server [" << m_ip << ':' << m_port << ']' << std::endl;
    }
    return true;	
}

void TCPClient::read()
{
    
    while (connectState)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        boost::array<uint8_t, 1024> buffer{0};

        if (error == nullptr) 
        {
            size_t len = socket->read_some(boost::asio::buffer(buffer), error);
        
            if (error == nullptr) 
            {
                if (len > 0)
                {
                    //cout << buffer.data() << endl;
                    if (onData)
                        onData(buffer.data(), static_cast<size_t>(len));
                }
            }
            else if (error)
                std::cout << "Disconnect to server [" << m_ip << ':' << m_port << ']' << std::endl;

        }   
    }
}



