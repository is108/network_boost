#pragma once

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>

using boost::asio::ip::udp;

class MulticastSender
{
public:
   MulticastSender(const std::string &multicast_ip, uint16_t multicast_port);
   ~MulticastSender();

public:
   
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
   boost::asio::ip::udp::endpoint ep;
   boost::scoped_ptr<udp::socket> socket;
   boost::asio::io_service io_service; 
   std::string m_ip;
   uint16_t m_port;
};
