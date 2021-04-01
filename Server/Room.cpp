/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** Room
*/

#include "Room.hpp"

Room::Room(std::string name, int port, IServer *room_tcp)
    : _name(name), _port(port), _room_tcp(room_tcp), isActive(true)
{
    _room = new UdpServer(_port, PACKET_SIZE_PACKET,
                          [this](std::string addr, void *packet) {
                              do_cmd(addr, packet);
                          });
    _room_thread = new std::thread([&] { _room->run(); });
    _room_game_thread = new std::thread([&, this] { start_room(); });
}

Room::~Room()
{
    isActive = false;
    _room_game_thread->join();
    _room->stop();
    _room_thread->join();
    delete _room;
}

void Room::join(std::pair<std::string, std::string> user)
{
    std::cout << user.second << " join the room " << _name << std::endl;
    entity to_send;
    for (auto entity: _manager.getEntities()) {
        to_send.name = entity->getName();
        to_send.sPos(entity->getComponent<Transform>()._position.x, entity->getComponent<Transform>()._position.y);
        to_send.sHp(entity->getComponent<Health>().getHP());
        to_send.sWeapon(entity->getComponent<Weapon>().getName());
        Packet p;
        p.setType(PACKET_SPAWN);
        p.setEntity(to_send);
        _room_tcp->writeTo(user.first, p.toCharString());
    }
    _users.push_back(user);
    create_player(user.second, 100, 500);
}

std::vector<std::pair<std::string, std::string>> Room::getUsers()
{
    return _users;
}

void Room::do_cmd(std::string addr, void *packet)
{
    Packet p;
    p.fromCharString((char *)packet, PACKET_PROVENANCE_CLIENT);
    switch (p.getType())
    {
    case PACKET_UPDATE:
    {
        _mutex_room.lock();
        _update_packets.push_back(p);
        _mutex_room.unlock();
    }
    break;
    default:
        break;
    }
}

void Room::removeUser(std::string u)
{
    for (auto user : _users)
    {
        if (user.first == u)
        {
            _mutex_room.lock();
            for (auto entity: _manager.getEntities())
                if (entity->getName() == user.second) {
                    entity->destroy();
                    break;
                }
            _mutex_room.unlock();
            std::cout << "User " << user.second << " leave the room " << _name << std::endl;
            _users.erase(std::find(_users.begin(), _users.end(), user));
            break;
        }
    }
}

bool Room::isEmpty()
{
    if (_users.size() == 0)
    {
        return true;
    }
    return false;
}

void Room::start_room()
{
    std::chrono::milliseconds timespan(1000 / 60);
    while (isActive)
    {
        updateRoom();
        updateClient();
		_manager.refresh();
		_manager.update();
        std::this_thread::sleep_for(timespan);
    }
}

void Room::create_player(std::string name, int x, int y, std::string wep)
{
    weapon to_put(wep);
    _mutex_room.lock();
    auto &player(_manager.addEntity("Player", name));
    player.addComponent<Transform>(x, y);
    player.addComponent<Health>(100);
    player.addComponent<Weapon>(to_put._text, to_put._name, to_put._power, to_put._speed, to_put._ammo, to_put._fireRate);
    _mutex_room.unlock();
    entity to_send;
    to_send.sName(name);
    to_send.sPos(x, y);
    to_send.sHp(player.getComponent<Health>().getHP());
    to_send.sWeapon(to_put._name);
    Packet p;
    p.setType(PACKET_SPAWN);
    p.setEntity(to_send);
    for (auto user: _users)
        _room_tcp->writeTo(user.first, p.toCharString());
}

void Room::updateRoom()
{
    _mutex_room.lock();
    for (auto update : _update_packets)
    {
        for (auto client_entitiy : update.getEntities())
        {
            for (auto room_entity : _manager.getEntities())
            {
                if (client_entitiy.name == room_entity->getName())
                {
                    room_entity->getComponent<Transform>().setPos(client_entitiy.x, client_entitiy.y);
                    room_entity->getComponent<Health>().setHP(client_entitiy.health);
                    if (client_entitiy.weapon != room_entity->getComponent<Weapon>().getName()) {
                        weapon to_put(client_entitiy.weapon);
                        room_entity->getComponent<Weapon>().setWeapon(to_put._text, to_put._name, to_put._power, to_put._speed, to_put._ammo, to_put._fireRate);
                    }
                    break;
                }
            }
        }
    }
    _update_packets.clear();
    _mutex_room.unlock();
}

void Room::updateClient()
{
    _mutex_room.lock();
    if (_manager.getEntities().size() == 0) {
        _mutex_room.unlock();
        return;
    }
    Packet p;
    p.setType(PACKET_UPDATE);
    for (auto ent : _manager.getEntities())
    {
        entity entity_to_send;
        entity_to_send.sName(ent->getName());
        entity_to_send.sPos(ent->getComponent<Transform>()._position.x, ent->getComponent<Transform>()._position.y);
        entity_to_send.sHp(ent->getComponent<Health>().getHP());
        entity_to_send.sWeapon(ent->getComponent<Weapon>().getName());
        p.addEntity(entity_to_send);
    }
    p.setDataSize(_manager.getEntities().size());
    _room->writeToAll(p.toCharString());
    _mutex_room.unlock();
}

void Room::writeToAllExcept(const std::string &user, void *p)
{
	for (auto otheruser: _users)
		if (otheruser.first != user)
			_room_tcp->writeTo(otheruser.first, p);
}
