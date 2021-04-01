/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** UdpClient
*/

#include "UdpClient.hpp"

UdpClient::UdpClient(int port, std::string ip, int buffer_size, std::function<void(std::string, void *)> read_callback)
: _socket(_io_context, udp::endpoint(udp::v4(), port)), _remote(address::from_string(ip), port), _buffer_size(buffer_size), _read_callback(read_callback)
{
    _read = (char *)malloc(sizeof(char) * buffer_size);
    recive();
}

void UdpClient::recive()
{
    _socket.async_receive_from(
        boost::asio::buffer(_read, _buffer_size), read_endpoint,
        [&, this](boost::system::error_code ec, size_t) {
            if (!ec) {
                _read_callback(read_endpoint.address().to_string(), _read);
                recive();
            }
            else
                std::cout << "Receive: failed" << std::endl;
        });
}

void UdpClient::write(void *packet)
{
    _socket.async_send_to(
        boost::asio::buffer(packet, _buffer_size), _remote,
        [this](boost::system::error_code ec, size_t) {
            if (ec)
                std::cout << "Faild to write" << std::endl;
        });
}

void UdpClient::run()
{
    _io_context.run();
}

void UdpClient::stop()
{
    _io_context.stop();
}