/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** Game
*/

#include "Game.hpp"

Game::Game(char **av)
: _name(av[3]), _server_ip(av[1])
{
	_udp = nullptr;
    _tcp = new TcpClient(av, PACKET_SIZE_PACKET, 
        [this](){
            connect();
        },
        [this](std::string addr, void *packet){
            do_tcp_cmd(addr, packet);
        });
    _tcp_thread = new std::thread([&]{_tcp->run();});
    _win = new sf::RenderWindow(sf::VideoMode(1920, 1000), "R-Type");
	run_game();
}

Game::~Game()
{
    if (_udp != nullptr) {
        Packet p;
        p.setType(PACKET_USERNAME);
        p.setUsername("DISCONNECT");
        _udp->write(p.toCharString(PACKET_PROVENANCE_CLIENT));
        sf::sleep(sf::milliseconds(100));
        _udp->stop();
        _udp_thread->join();
    }
    _tcp->stop();
    _tcp_thread->join();
}

void Game::connect()
{
    Packet p;
    p.setType(PACKET_USERNAME);
    p.setUsername(_name);
    _tcp->write(p.toCharString(PACKET_PROVENANCE_CLIENT));
}

void Game::do_tcp_cmd(std::string addr, void *packet)
{
    Packet p;
    p.fromCharString((char *)packet, PACKET_PROVENANCE_SERVER);
    switch (p.getType())
    {
    case PACKET_CREATED_ROOM: {
        _udp = new UdpClient(p.getCreatedPort(), _server_ip, PACKET_SIZE_PACKET,
        [this](std::string addr, void *packet) {
            do_udp_cmd(addr, packet);
        });
        _udp_thread = new std::thread([&]{_udp->run();});
		Packet p;
        _udp->write(p.toCharString(PACKET_PROVENANCE_CLIENT));
        }
        break;
    case PACKET_LIST_ROOMS: {
        setSceneSelection(p);
    }
	break;
    case PACKET_SPAWN: {
		if (p.getEntity().name != _player->getName())
	        createEntity(p.getEntity());
	}
	break;
	case PACKET_USERNAME: {
		if (p.getUsername().find("OFF_") != std::string::npos) {
			std::string toDel = p.getUsername().substr(p.getUsername().find('_')+1, p.getUsername().size()-p.getUsername().find('_')-1);
			for (auto entity: _manager.getEntities())
				if (entity->getName() == toDel)
					entity->destroy();
		}
	}
	break;
    case PACKET_SHOOT: {
        for (auto entity: _manager.getEntities()) {
            if (p.getShooter() == entity->getName()) {
                shoot(entity);
                break;
            }
        }
    }
	break;
    default:
        break;
    }
}

void Game::do_udp_cmd(std::string addr, void *packet)
{
	Packet p;
    p.fromCharString((char *)packet, PACKET_PROVENANCE_SERVER);
	switch (p.getType())
	{
	case PACKET_UPDATE: {
        _game_mutex.lock();
		_update_packets.push_back(p);
        _game_mutex.unlock();
    }
	break;
    case PACKET_SPAWN: {
		if (p.getEntity().name != _player->getName())
	        createEntity(p.getEntity());
	}
	break;
	default:
		break;
	}
}

void Game::shoot(Entity *parent)
{
    auto &bullet(_manager.addEntity("Bullet", parent->getComponent<Weapon>().getName() + " bullet", parent));
    bullet.addComponent<SpriteComponent>(parent->getComponent<Weapon>().getTexture(), _win);
    if (parent->getType() == "Player") {
        bullet.getComponent<Transform>().setPos(parent->getComponent<Transform>()._position.x + parent->getComponent<Colider>().getColider().width + 1
                                        , parent->getComponent<Transform>()._position.y + parent->getComponent<Colider>().getColider().height / 2);
        bullet.getComponent<Transform>()._velocity.x = 1;
    }
    else {
        bullet.getComponent<Transform>().setPos(parent->getComponent<Transform>()._position.x - bullet.getComponent<SpriteComponent>().getTextureRect().width - 1
                                        , parent->getComponent<Transform>()._position.y + parent->getComponent<Colider>().getColider().height / 2);
        bullet.getComponent<Transform>()._velocity.x = -1;
        bullet.getComponent<SpriteComponent>().setRotation(180);
    }
    bullet.getComponent<Transform>()._speed = parent->getComponent<Weapon>().getSpeed();
    bullet.addComponent<Colider>(Colider::OVERLAP, [&](Entity *colid) {
    });
}

void Game::run_game()
{
    setSceneLobby();
    sf::Clock cadence;
    sf::Clock timeexec;
    while (_win->isOpen())
    {
        timeexec.restart();
        sf::Event event;
        while (_win->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                _win->close();
            if (event.type == sf::Event::KeyReleased){
                if (event.key.code == sf::Keyboard::I)
                    _player->getComponent<Health>().setHP(_player->getComponent<Health>().getHP() - 5);
                if (event.key.code == sf::Keyboard::U) {
                    weapon to_put("WEP_normal");
                    _player->getComponent<Weapon>().setWeapon(to_put._text, to_put._name, to_put._power, to_put._speed, to_put._ammo, to_put._fireRate);
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (_player->getComponent<Weapon>().getAmmo() > 0 && cadence.getElapsedTime().asMilliseconds() > _player->getComponent<Weapon>().getFireRate()) {
                Packet p;
                p.setType(PACKET_SHOOT);
                p.setShooter(_player->getName());
                _tcp->write(p.toCharString());
                shoot(_player);
                _player->getComponent<Weapon>().useAmmo();
                cadence.restart();
            }
        }
        _win->clear();
        update();
        isColiding();
        _manager.update();
        _manager.draw();
        _manager.refresh();
        _win->display();
        sf::sleep(sf::microseconds((1000000/60) - timeexec.getElapsedTime().asMicroseconds()));
    }
}

void Game::isColiding()
{
    for (auto toCheck: _manager.getEntities()) {
        if (toCheck->getType() == "Player" || toCheck->getType() == "Button" || toCheck->getType() == "Monster") {
            if (toCheck->hasComponent<Colider>()) {
                for (auto entity: _manager.getEntities()) {
                    if (entity != toCheck && entity->hasComponent<Colider>())
                        toCheck->getComponent<Colider>().isColidingWith(entity);
                }
            }
        }
    }
}

void Game::update()
{
    _game_mutex.lock();
    for (auto packet: _update_packets) {
        for (auto server_entity: packet.getEntities()) {
            if (server_entity.name != _player->getName()) {
                for (auto client_entity: _manager.getEntities()) {
                    if (server_entity.name == client_entity->getName()) {
                        client_entity->getComponent<Transform>().setPos(server_entity.x, server_entity.y);
                        client_entity->getComponent<Health>().setHP(server_entity.health);
                        if (server_entity.weapon != client_entity->getComponent<Weapon>().getName()) {
                            weapon to_put(server_entity.weapon);
                            client_entity->getComponent<Weapon>().setWeapon(to_put._text, to_put._name, to_put._power, to_put._speed, to_put._ammo, to_put._fireRate);
                        }
                        break;
                    }
                }
            }
        }
    }
    _update_packets.clear();
    _game_mutex.unlock();
}

void Game::createEntity(entity toCreate)
{
    if (toCreate.name.find("Monster") != std::string::npos)
        createMonster("../Assets/monster.png", toCreate.name, toCreate.x, toCreate.y);
    else
        createPlayer(toCreate.name, false, toCreate.x, toCreate.y, toCreate.health, toCreate.weapon);
}

void Game::createButton(std::string name, int x, int y)
{
    auto &button(_manager.addEntity("Button", name));
    button.addComponent<Transform>(x, y);
    button.addComponent<Button>(_win);
    button.addComponent<Colider>(Colider::OVERLAP, [&, this](Entity *colid) {
        if (colid->getName() == _player->getName()) {
            if (button.getName() == "Create room") {
                Packet p1;
                p1.setType(PACKET_CREATE_ROOM);
                p1.setToCreate("court");
                _tcp->write(p1.toCharString(PACKET_PROVENANCE_CLIENT));
                setSceneRoom();
            }
            if (button.getName() == "Get rooms") {
                Packet to_send;
                to_send.setType(PACKET_GET_ROOMS);
                _tcp->write(to_send.toCharString(PACKET_PROVENANCE_CLIENT));
                button.destroy();
            }
            if (button.getName() == "Leave room") {
                Packet p;
                p.setType(PACKET_USERNAME);
                p.setUsername("DISCONNECT");
                _tcp->write(p.toCharString(PACKET_PROVENANCE_CLIENT));
                _udp->stop();
                _udp_thread->join();
                delete _udp;
                delete _udp_thread;
                _udp = nullptr;
                _udp_thread = nullptr;
                setSceneLobby();
            }
            if (button.getName().find("Join room") != std::string::npos) {
                room to_join(button.getName().substr(button.getName().find(':')+1, button.getName().find(':',button.getName().find(':')+1)-button.getName().find(':')-1)
                            , std::atoi(button.getName().substr(button.getName().find_last_of(':')+1, button.getName().size()-button.getName().find_last_of(':')-1).c_str()));
                Packet p;
                p.setType(PACKET_JOIN_ROOM);
                p.setRoomToJoin(to_join);
                _tcp->write(p.toCharString(PACKET_PROVENANCE_CLIENT));
                _udp = new UdpClient(to_join.port, _server_ip, PACKET_SIZE_PACKET,
                [this](std::string addr, void *packet) {
                    do_udp_cmd(addr, packet);
                });
                _udp_thread = new std::thread([&]{_udp->run();});
                Packet ping;
                _udp->write(ping.toCharString(PACKET_PROVENANCE_CLIENT));
                setSceneRoom();
            }
            if (button.getName() == "Return menu")
                setSceneLobby();
        }
    });
}

void Game::setSceneLobby()
{
    for (auto entity: _manager.getEntities())
        entity->destroy();
    _player = createPlayer(_name, true, 500, 500);
    createButton("Create room", 1500, 300);
    createButton("Get rooms", 1500, 700);
}

void Game::setSceneSelection(Packet rooms)
{
    for (auto entity: _manager.getEntities())
        entity->destroy();
    int i = 0;
    _player = createPlayer(_name, true, 500, 500);
    createButton("Return menu", 10, 10);
    for (auto room: rooms.getRooms()) {
        createButton(std::string("Join room") + ":" + room.name + ":" + std::to_string(room.port), 1200, 300 + (i * 200));
        i++;
    }
}

void Game::setSceneRoom()
{
    for (auto entity: _manager.getEntities())
        entity->destroy();
    _player = createPlayer(_name, true, 50, 500);
    _player->addComponent<HUD>(_win);
    createButton("Leave room", 10, 10);
}


Entity *Game::createPlayer(std::string name, bool isControl, int x, int y, int health, std::string wep)
{
    weapon to_put(wep);
    auto &player(_manager.addEntity("Player", name));
    player.addComponent<Transform>(x, y);
    player.getComponent<Transform>()._speed = 10;
    player.addComponent<Health>(health);
    player.addComponent<SpriteComponent>("../Assets/amster.png", _win);
    player.addComponent<Weapon>(to_put._text, to_put._name, to_put._power, to_put._speed, to_put._ammo);
	player.addComponent<Colider>(Colider::OVERLAP, [&, this](Entity *colid) {
        if (colid->getType() == "Bullet") {
            if (colid->getParent()->getName() == player.getName() && &player != _player)
            {
            }
            else if (colid->getParent()->getName() == _player->getName() && &player == _player)
            {
            }
            else if (colid->getParent()->getName() != _player->getName())
            {
                if (player.getName() == _player->getName())
                    _player->getComponent<Health>().setHP(_player->getComponent<Health>().getHP() - colid->getParent()->getComponent<Weapon>().getPower());
                colid->destroy();
            }
            else if (colid->getParent()->getName() != player.getName())
            {
                colid->destroy();
            }
        }
	});
    if (isControl) {
        player.addComponent<KeyInput>();
        player.addComponent<NetworkReplicate>(&_udp);
        return &player;
    }
    else {
        player.addComponent<LifeBar>(_win);
        return nullptr;
    }
}

void Game::createMonster(std::string texture, std::string name, int x, int y, int health)
{
    auto &monster(_manager.addEntity("Monster", name));
    monster.addComponent<Transform>(x, y);
    monster.addComponent<SpriteComponent>(texture, _win);
    monster.addComponent<Health>(health);
    monster.addComponent<LifeBar>(_win);
    monster.addComponent<Colider>(Colider::OVERLAP, [&](Entity *colid) {
    });
    monster.addComponent<Weapon>();
}
