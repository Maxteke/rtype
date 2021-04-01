/*
** EPITECH PROJECT, 2020
** B-CPP-500-LYN-5-1-babel-maxence.pellerin
** File description:
** Server
*/

#include "TcpServer.hpp"

TcpServer::TcpServer(int port, int buffer_size, std::function<void(std::string)> accept_callback, std::function<void(std::string)> leave_callback, std::function<void(std::string, void *)> read_callback)
: _endpoint(tcp::v4(), port), _acceptor(_io_context, _endpoint), _buffer_size(buffer_size), _accept_callback(accept_callback), _leave_callback(leave_callback), _read_callback(read_callback)
{
    do_accept();
}

TcpServer::~TcpServer()
{
}

void TcpServer::run()
{
    _io_context.run();
}

void TcpServer::stop()
{
    _io_context.stop();
}

void TcpServer::do_accept()
{
    _acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            User new_user = std::make_shared<Session>(std::move(socket), this, _buffer_size, _leave_callback, _read_callback);
            _users.push_back(new_user);
            _accept_callback(new_user->_ip_address);
            do_accept();
        });
}

void TcpServer::writeTo(std::string addr, void *packet)
{
    for (int i = 0; i < _users.size(); i++) {
        if (_users[i]->_ip_address == addr) {
            _users[i]->write(packet);
            break;
        }
    }
}

void TcpServer::writeToAll(void *packet)
{
    for (auto user: _users)
        user->write(packet);
}

void TcpServer::delUser(Session *toDel) {
    for (int i = 0; i < _users.size(); i++) {
        if (_users[i]->_ip_address == toDel->_ip_address) {
            _users.erase(_users.begin() + i);
            break;
        }
    }
}
