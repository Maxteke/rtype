/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** NetworkReplicate
*/

#ifndef NETWORKREPLICATE_HPP_
#define NETWORKREPLICATE_HPP_

#include "Components.hpp"
#include "Boost/IClient.hpp"
#include "Shared/Packet.hpp"

class NetworkReplicate : public Component {
    public:
        NetworkReplicate(IClient **udp): _udp(udp){}
        void init() override
        {
            _transform = nullptr;
            _health = nullptr;
            if (_entity->hasComponent<Transform>())
                _transform = &_entity->getComponent<Transform>();
            if (_entity->hasComponent<Health>())
                _health = &_entity->getComponent<Health>();
            if (_entity->hasComponent<Weapon>())
                _weapon = &_entity->getComponent<Weapon>();
        }

        void update() override
        {
            if (*_udp) {
                entity to_send;
                to_send.sName(_entity->getName());
                if (_transform)
                    to_send.sPos(_transform->_position.x, _transform->_position.y);
                if (_health)
                    to_send.sHp(_health->getHP());
                if (_weapon)
                    to_send.sWeapon(_weapon->getName());
                Packet p;
                p.setType(PACKET_UPDATE);
                p.addEntity(to_send);
                p.setDataSize(1);
                (*_udp)->write(p.toCharString());
            }
        }

    private:
        Transform *_transform;
        Weapon *_weapon;
        Health *_health;
        IClient **_udp;
};


#endif /* !NETWORKREPLICATE_HPP_ */
