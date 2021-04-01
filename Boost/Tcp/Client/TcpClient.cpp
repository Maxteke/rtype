/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** TcpClient
*/

#include "TcpClient.hpp"

TcpClient::TcpClient(char **av, size_t buffer_size, std::function<void()> connect_callback, std::function<void(std::string, void *)> read_callback)
: _io_context(new boost::asio::io_context()), _socket(*_io_context), _buffer_size(buffer_size), _connect_callback(connect_callback), _read_callback(read_callback)
{
    _buffer = (char *)malloc(sizeof(char) * _buffer_size);
    tcp::resolver resolver(*_io_context);
    connect(resolver.resolve(av[1], av[2]));
}

TcpClient::~TcpClient()
{
    _socket.close();
}

void TcpClient::write(void *packet)
{
    boost::asio::async_write(_socket,
        boost::asio::buffer(packet, _buffer_size),
        [this](boost::system::error_code ec, std::size_t) {
            if (ec) {
                _socket.close();
                std::cout << "Write: Can't find server" << std::endl;
                exit(0);
            }
        });
}

void TcpClient::run()
{
    _io_context->run();
}

void TcpClient::stop()
{
    _io_context->stop();
}

void TcpClient::connect(const tcp::resolver::results_type &endpoints)
{
    boost::asio::async_connect(_socket, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint)
        {
            if (!ec) {
                _connect_callback();
                read();
            }  
            else {
                std::cout << "Can't find server" << std::endl;
                exit(0);
            }
        });
}

void TcpClient::read()
{
    boost::asio::async_read(_socket,
        boost::asio::buffer(_buffer, _buffer_size),
        [this](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                _read_callback(_socket.remote_endpoint().address().to_string(), _buffer);
				read();
            }
            else {
                _socket.close();
                std::cout << "Connection lost" << std::endl;
                exit(0);
            }
        });
}
