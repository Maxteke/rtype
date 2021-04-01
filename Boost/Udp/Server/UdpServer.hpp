/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** UdpServer
*/

#ifndef UDPSERVER_HPP_
#define UDPSERVER_HPP_

#include "../../IServer.hpp"
#include <boost/asio.hpp>
#include <iostream>
using boost::asio::ip::udp;

class UdpServer : public IServer{
    public:
        UdpServer(int port, int buffer_size, std::function<void(std::string, void *)> read_callback);
        void writeToAll(void *packet);
        void run();
        void stop();
        void writeTo(std::string addr, void *packet);
        void remove_user(std::string addr) override;
    private:
        void recive();
        bool isInUser(std::string addr);
        udp::endpoint read_endpoint;
        std::vector<udp::endpoint> _users;
        boost::asio::io_context _io_context;
        udp::socket _socket;
        char *_read;
        int _buffer_size;
        std::function<void(std::string, void *)> _read_callback;
};

#endif /* !UDPSERVER_HPP_ */
