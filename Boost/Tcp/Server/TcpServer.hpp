/*
** EPITECH PROJECT, 2020
** B-CPP-500-LYN-5-1-babel-maxence.pellerin
** File description:
** Server
*/

#ifndef TCPSERVER_HPP_
#define TCPSERVER_HPP_

#include "../../IServer.hpp"
#include "Session.hpp"
#include <vector>

typedef std::shared_ptr<Session> User;

class TcpServer : public IServer {
    public:
        TcpServer(int port, int buffer_size, std::function<void(std::string)> accept_callback, std::function<void(std::string)> leave_callback, std::function<void(std::string, void *)> read_callback);
        ~TcpServer() override;
        void run();
        void stop();
        std::vector<User> _users;
        void writeTo(std::string addr, void *packet);
        void writeToAll(void *packet);
        void delUser(Session *user);
        void remove_user(std::string addr){};
    private:
        void do_accept();
        boost::asio::io_context _io_context;
        tcp::endpoint _endpoint;
        tcp::acceptor _acceptor;
        std::size_t _buffer_size;
        std::function<void(std::string)> _accept_callback;
        std::function<void(std::string)> _leave_callback;
        std::function<void(std::string, void *)> _read_callback;
};

#endif /* !TCPSERVER_HPP_ */
