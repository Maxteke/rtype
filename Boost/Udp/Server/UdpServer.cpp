/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** UdpServer
*/

#include "UdpServer.hpp"

UdpServer::UdpServer(int port, int buffer_size, std::function<void(std::string, void *)> read_callback)
: _socket(_io_context, udp::endpoint(udp::v4(), port)), _buffer_size(buffer_size), _read_callback(read_callback)
{
    _read = (char *)malloc(sizeof(char) * buffer_size);
    recive();
}

void UdpServer::recive()
{
    _socket.async_receive_from(
        boost::asio::buffer(_read, _buffer_size), read_endpoint,
        [&, this](boost::system::error_code ec, std::size_t bytes_recvd){
            if (!ec) {
                if (!isInUser(read_endpoint.address().to_string()))
                    _users.push_back(read_endpoint);
                _read_callback(read_endpoint.address().to_string(), _read);
                recive();
            }
            else {
                std::cout << "Recieve failed. removing user" << std::endl;
                remove_user(read_endpoint.address().to_string());
            }
        });
}

bool UdpServer::isInUser(std::string addr)
{
    bool isIn = false;

    for (auto user: _users)
        if (user.address().to_string() == addr)
            isIn = true;
    return isIn;
}

void UdpServer::writeToAll(void *packet)
{
    for (auto user: _users) {
        _socket.async_send_to(
            boost::asio::buffer(packet, _buffer_size), user,
            [&, this](boost::system::error_code ec, std::size_t){
                if (ec) {
                    std::cout << "Send: write faild" << std::endl;
                    remove_user(user.address().to_string());
                }
            });
    }
}

void UdpServer::run()
{
    _io_context.run();
}

void UdpServer::stop()
{
    _io_context.stop();
}

void UdpServer::writeTo(std::string addr, void *packet)
{
    for (auto user: _users) {
        if (user.address().to_string() == addr) {
            _socket.async_send_to(
                boost::asio::buffer(packet, _buffer_size), user,
                [&, this](boost::system::error_code ec, std::size_t){
                    if (ec) {
                        std::cout << "Send: write faild" << std::endl;
                        remove_user(user.address().to_string());
                    }
                });
            break;
        }
    }
}


void UdpServer::remove_user(std::string addr)
{
    for (auto user: _users) {
        if (user.address().to_string() == addr) {
            _users.erase(std::find(_users.begin(), _users.end(), user));
            break;
        }
    }
}