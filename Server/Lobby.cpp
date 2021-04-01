/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** Lobby
*/

#include "Lobby.hpp"

Lobby::Lobby(char **av)
    : _port(std::atoi(av[1]))
{
    for (int i = 0; i < 5; i++)
        _ports.push_back(_port + 1 + i);
    _lobby = new TcpServer(
        _port, PACKET_SIZE_PACKET,
        [this](std::string addr) {
            user_join(addr);
        },
        [this](std::string addr) {
            user_leave(addr);
        },
        [this](std::string addr, void *packet) {
            do_cmd(addr, packet);
        });
    _lobby->run();
}

void Lobby::user_join(std::string addr)
{
    std::cout << "New connection: " << addr << "." << std::endl;
}

void Lobby::do_cmd(std::string addr, void *packet)
{
    _mutex_lobby.lock();
    std::pair<std::string, std::string> tmp;
    for (auto user : _users)
        if (user.first == addr)
        {
            tmp = user;
            break;
        }
    Packet p;
    p.fromCharString((char *)packet, PACKET_PROVENANCE_CLIENT);
    switch (p.getType())
    {
    case PACKET_USERNAME:
    {
        if (p.getUsername() == std::string("DISCONNECT"))
        {
            user_leave_room(addr);
        }
        else
        {
            std::cout << addr << " username: " << p.getUsername() << std::endl;
            _users.push_back(std::make_pair(addr, p.getUsername()));
        }
    }
    break;
    case PACKET_CREATE_ROOM:
    {
        Packet to_send;
        to_send.setType(PACKET_CREATED_ROOM);
        to_send.setCreatedPort(create_room(p.getToCreate(), tmp)->getPort());
        _lobby->writeTo(addr, to_send.toCharString(PACKET_PROVENANCE_SERVER));
    }
    break;
    case PACKET_GET_ROOMS:
    {
        Packet to_send;
        to_send.setType(PACKET_LIST_ROOMS);
        to_send.setDataSize(_rooms.size());
        for (auto game_room : _rooms)
        {
            room to_send_room(game_room->getName(), game_room->getPort());
            to_send.addRoom(to_send_room);
        }
        _lobby->writeTo(addr, to_send.toCharString(PACKET_PROVENANCE_SERVER));
    }
    break;
    case PACKET_JOIN_ROOM:
        join_room(p.getRoomToJoin().name, tmp);
    break;
	case PACKET_SHOOT:
	{
        for (auto game_room : _rooms)
			for (auto user: game_room->getUsers())
				if (user.first == addr) {
					game_room->writeToAllExcept(addr, p.toCharString());
					break;
				}
	}
	break;
    default:
        break;
    }
    _mutex_lobby.unlock();
}

void Lobby::user_leave(std::string addr)
{
    std::cout << addr << " disconect." << std::endl;
    _mutex_lobby.lock();
    user_leave_room(addr);
    _mutex_lobby.unlock();
}

void Lobby::user_leave_room(std::string addr)
{
    for (auto room : _rooms)
    {
        for (auto user : room->getUsers())
        {
            if (user.first == addr)
            {
                _users.push_back(user);
                room->removeUser(addr);
                Packet p;
                p.setType(PACKET_USERNAME);
                p.setUsername(std::string("OFF_") + user.second);
                for (auto otheruser : room->getUsers())
                    _lobby->writeTo(otheruser.first, p.toCharString());
                break;
            }
        }

        if (room->isEmpty())
        {
            _ports.push_back(room->getPort());
            _rooms.erase(std::find(_rooms.begin(), _rooms.end(), room));
            std::cout << "Room " << room->getName() << " closed" << std::endl;
            delete room;
            std::cout << "Number of room: " << _rooms.size() << std::endl;
        }
    }
}

Room *Lobby::create_room(std::string name, std::pair<std::string, std::string> user)
{
    if (_ports.size() > 0)
    {
        Room *tmp = new Room(name, _ports[_ports.size() - 1], _lobby);
        std::cout << "Room " << tmp->getName() << " created on port: " << tmp->getPort() << std::endl;
        _ports.pop_back();
        tmp->join(user);
        _users.erase(std::find(_users.begin(), _users.end(), user));
        _rooms.push_back(tmp);
        std::cout << "Number of room: " << _rooms.size() << std::endl;
        return tmp;
    }
    else
    {
        std::cout << "No port availible" << std::endl;
        return nullptr;
    }
}

void Lobby::join_room(std::string name, std::pair<std::string, std::string> user)
{
    for (auto room : _rooms)
        if (room->getName() == name)
        {
            room->join(user);
            _users.erase(std::find(_users.begin(), _users.end(), user));
            break;
        }
}
