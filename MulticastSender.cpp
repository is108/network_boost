#include "MulticastSender.h"
#include <iostream>
MulticastSender::MulticastSender(const std::string &multicast_ip, uint16_t multicast_port)
   : m_ip(multicast_ip)
   , m_port(multicast_port) 
   , ep(boost::asio::ip::address::from_string(multicast_ip), multicast_port)
{
}

void MulticastSender::connect()
{
   socket.reset(new udp::socket(io_service, ep.protocol()));
}

void MulticastSender::disconnect()
{
   socket.reset(NULL);
}

MulticastSender::~MulticastSender()
{
   disconnect();
}

bool MulticastSender::send(const uint8_t *data, size_t size)
{
   socket->send_to(boost::asio::buffer(data, size), ep);

   return true;
}