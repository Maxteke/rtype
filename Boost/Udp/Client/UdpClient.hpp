/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** UdpClient
*/

#ifndef UDPCLIENT_HPP_
#define UDPCLIENT_HPP_

#include "../../IClient.hpp"
#include <boost/asio.hpp>
#include <iostream>
using boost::asio::ip::udp;
using boost::asio::ip::address;

class UdpClient : public IClient {
    public:
        UdpClient(int port, std::string ip, int buffer_size, std::function<void(std::string, void *)> read_callback);
        void recive();
        void write(void *packet);
        void run();
        void stop();
    private:
        boost::asio::io_context _io_context;
        udp::socket _socket;
        udp::endpoint _remote;
        udp::endpoint read_endpoint;
        char *_read;
        int _buffer_size;
        std::function<void(std::string, void *)> _read_callback;
};

#endif /* !UDPCLIENT_HPP_ */
