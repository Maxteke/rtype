/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** Game
*/

#ifndef GAME_HPP_
#define GAME_HPP_

#include "Boost/Tcp/Client/TcpClient.hpp"
#include "Boost/Udp/Client/UdpClient.hpp"
#include "Shared/Packet.hpp"
#include "Shared/WeaponRef.hpp"
#include "ECS/Components.hpp"
#include "SFML/Graphics.hpp"
#include <thread>
#include <deque>

class Game {
    public:
        Game(char **av);
        ~Game();
        void connect();
        void do_tcp_cmd(std::string addr, void *packet);
        void do_udp_cmd(std::string addr, void *packet);
		void run_game();
    private:
        void isColiding();
        void update();
        void createButton(std::string name, int x = 0, int y = 0);
        Entity *createPlayer(std::string name, bool isControl, int x = 0, int y = 0, int health = 100, std::string wep = "WEP_basic");
        void createMonster(std::string texture, std::string name, int x = 0, int y = 0, int health = 100);
        void createEntity(entity toCreate);
        void setSceneLobby();
        void setSceneRoom();
        void setSceneSelection(Packet rooms);
        void shoot(Entity *parent);
        EntityManager _manager;
        std::string _name;
        std::string _server_ip;
        IClient *_tcp;
        IClient *_udp;
        std::thread *_tcp_thread;
        std::thread *_udp_thread;
        sf::RenderWindow *_win;
        Entity *_player;
        std::vector<Packet> _update_packets;
        std::mutex _game_mutex;
};

#endif /* !GAME_HPP_ */
