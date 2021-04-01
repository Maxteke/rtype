/*
** EPITECH PROJECT, 2020
** B-CPP-500-LYN-5-1-babel-maxence.pellerin
** File description:
** Session
*/

#include "Session.hpp"

Session::Session(tcp::socket socket, TcpServer *server, std::size_t buffer_size, std::function<void(std::string)> leave_callback, std::function<void(std::string, void *)> callback)
: _socket(std::move(socket)), _buffer_size(buffer_size), _leave_callback(leave_callback), _callback(callback)
{
    _read = (char *)malloc(sizeof(char) * buffer_size);
    _server = server;
    _ip_address = _socket.remote_endpoint().address().to_string();
    read();
}

Session::~Session()
{
    if (_socket.is_open())
        _socket.close();
}

void Session::read()
{
    boost::asio::async_read(_socket,
        boost::asio::buffer(_read, _buffer_size),
        [this](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                _callback(_socket.remote_endpoint().address().to_string(), _read);
                read();
            }
            else {
                _leave_callback(_ip_address);
                _socket.close();
                _server->delUser(this);
				std::cout << "SOCKET ERROR : " << ec.message() << std::endl;
			}
        });
}

void Session::write(void *packet)
{
    boost::asio::async_write(_socket,
        boost::asio::buffer(packet, _buffer_size),
        [this](boost::system::error_code ec, std::size_t) {
			if (ec) {
                _leave_callback(_ip_address);
    	        std::cout << "Asynch write: " << ec.message() << std::endl;
				_socket.close();
                _server->delUser(this);
			}
        });
}