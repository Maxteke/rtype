/*
** EPITECH PROJECT, 2020
** B-CPP-500-LYN-5-1-babel-maxence.pellerin
** File description:
** Session
*/

#ifndef SESSION_HPP_
#define SESSION_HPP_

#include <boost/asio.hpp>
#include <iostream>
using boost::asio::ip::tcp;

class TcpServer;

class Session : public std::enable_shared_from_this<Session>{
    public:
        Session(tcp::socket, TcpServer *, std::size_t, std::function<void(std::string)> leave_callback, std::function<void(std::string, void *)> callback);
        ~Session();
        tcp::socket getSocket(){ return std::move(_socket);};
        void write(void *);
        std::string _ip_address;
    private:
        void read();
        tcp::socket _socket;
        TcpServer *_server;
        char *_read;
        std::size_t _buffer_size;
        std::function<void(std::string)> _leave_callback;
        std::function<void(std::string, void *)> _callback;

};

#include "TcpServer.hpp"

#endif /* !SESSION_HPP_ */
