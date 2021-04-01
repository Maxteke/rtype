/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** Client
*/

#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include "../../IClient.hpp"
#include <boost/asio.hpp>
#include <iostream>
using boost::asio::ip::tcp;

class TcpClient : public IClient {
    public:
        TcpClient(char **, size_t, std::function<void()> connect_callback, std::function<void(std::string, void *)> read_callback);
        ~TcpClient() override;
		void write(void *);
        void run();
        void stop();
    private:
        void connect(const tcp::resolver::results_type &);
        void read();
        boost::asio::io_context *_io_context;
        tcp::socket _socket;
        size_t _buffer_size;
        char *_buffer;
        std::function<void()> _connect_callback;
        std::function<void(std::string, void *)> _read_callback;
};

#endif /* !TCPCLIENT_HPP_ */
