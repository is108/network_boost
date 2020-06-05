#include "UDPClient.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>

UDPClient::UDPClient( const std::string &ip, uint16_t port)
    : m_ip(ip)
    , m_port(port)
    , socket(io_service, udp::endpoint(udp::v4(), 0))
{
}

void UDPClient::connect()
{
    udp::resolver resolver(io_service);
    udp::resolver::query query(udp::v4(), m_ip, std::to_string(m_port));
    udp::resolver::iterator iter = resolver.resolve(query);
    receiver_endpoint = *iter;
}

void UDPClient::disconnect()
{
    socket.close();
}

UDPClient::~UDPClient()
{
    disconnect();
}

bool UDPClient::send(const uint8_t *data, size_t size)
{
    socket.send_to(boost::asio::buffer(data, size), receiver_endpoint);

    return true;
}

