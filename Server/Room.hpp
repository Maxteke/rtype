/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** Room
*/

#ifndef ROOM_HPP_
#define ROOM_HPP_

#include "Boost/Udp/Server/UdpServer.hpp"
#include "../Boost/Tcp/Server/TcpServer.hpp"
#include "Shared/Packet.hpp"
#include "Shared/WeaponRef.hpp"
#include "ECS/Components.hpp"
#include <chrono>

class Room {
    public:
        Room(std::string name, int port, IServer *room_tcp);
        ~Room();
        void join(std::pair<std::string, std::string> user);
        std::string getName() { return _name; }
        int getPort() { return _port; }
        std::vector<std::pair<std::string, std::string>> getUsers();
        void do_cmd(std::string addr, void *packet);
		void removeUser(std::string addr);
		bool isEmpty();
		void writeToAllExcept(const std::string &, void *);
    private:
        void start_room();
        void create_player(std::string name, int x, int y, std::string wep = "WEP_basic");
        void updateRoom();
        void updateClient();
        IServer *_room;
        std::string _name;
        int _port;
        std::vector<std::pair<std::string, std::string>> _users;
        std::thread *_room_thread;
        std::thread *_room_game_thread;
        IServer *_room_tcp;
        bool isActive;
        EntityManager _manager;
        std::vector<Packet> _update_packets;
        std::mutex _mutex_room;
};


#endif /* !ROOM_HPP_ */
