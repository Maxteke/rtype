/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** Lobby
*/

#ifndef LOBBY_HPP_
#define LOBBY_HPP_

#include "../Boost/Tcp/Server/TcpServer.hpp"
#include "Room.hpp"
#include "../Shared/Packet.hpp"
#include <vector>

class Lobby {
    public:
        Lobby(char **av);
    private:
        void user_join(std::string addr);
        void do_cmd(std::string addr, void *packet);
        void user_leave(std::string addr);
        Room *create_room(std::string name, std::pair<std::string, std::string> user);
        void join_room(std::string name, std::pair<std::string, std::string> user);
        void user_leave_room(std::string addr);
        int _port;
        IServer *_lobby;
        std::vector<std::pair<std::string, std::string>> _users;
        std::vector<Room *> _rooms;
        std::vector<int> _ports;
        std::thread *_watchdog;
        bool isActive;
        std::mutex _mutex_lobby;
};

#endif /* !LOBBY_HPP_ */
