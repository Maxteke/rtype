/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** Transform
*/

#ifndef Transform_HPP_
#define Transform_HPP_

#include "Components.hpp"
#include "SFML/Graphics.hpp"

class Transform : public Component {
    public:
        Transform()
        {
            _position.x = 0;
            _position.y = 0;
        }
        Transform(int x, int y)
        {
            _position.x = x;
            _position.y = y;
        }
        void init() override
        {
            canUP = true;
            canDOWN = true;
            canRIGHT = true;
            canLEFT = true;
            _velocity.x = 0;
            _velocity.y = 0;
        }
        void update() override
        {
            _position.x += _velocity.x * _speed;
            _position.y += _velocity.y * _speed;
            if (_position.x > 1920 || _position.x < 0)
                _entity->destroy();
        }
        void setPos(int x, int y)
        {
            _position.x = x;
            _position.y = y;
        }

        void setFreeToMoove()
        {
            canUP = true;
            canDOWN = true;
            canRIGHT = true;
            canLEFT = true;
        }

        sf::Vector2f _position;
        sf::Vector2i _velocity;
        int _speed = 1;
        bool canUP;
        bool canLEFT;
        bool canRIGHT;
        bool canDOWN;
};

#endif /* !Transform_HPP_ */
