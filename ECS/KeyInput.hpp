/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** KeyInput
*/

#ifndef KEYINPUT_HPP_
#define KEYINPUT_HPP_

#include "Components.hpp"
#include "SFML/Graphics.hpp"


class KeyInput : public Component {
    public:
        KeyInput(){};
        void init() override
        {
            if (!_entity->hasComponent<Transform>())
                _entity->addComponent<Transform>();
            _transform = &_entity->getComponent<Transform>();
        }
		bool canDir(const std::string &dir, Transform *t)
		{
			if (dir == "north")
				if (t->_position.y - t->_speed <= 0)
					return false;
			if (dir == "east")
				if (t->_position.x + t->_speed >= 1920)
					return false;
			if (dir == "south")
				if (t->_position.y + t->_speed >= 1080)
					return false;
			if (dir == "west")
				if (t->_position.x - t->_speed <= 0)
					return false;
			return true;
		}

        void update() override
        {
            if (_transform->_position.y > 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && _transform->canUP && canDir("north",_transform))
                _transform->_velocity.y = -1;
            else if (_transform->_position.y + _entity->getComponent<SpriteComponent>().getTextureRect().height < 1000 
                    && sf::Keyboard::isKeyPressed(sf::Keyboard::S) && _transform->canDOWN && canDir("south",_transform))
                _transform->_velocity.y = 1;
            else
                _transform->_velocity.y = 0;
            if (_transform->_position.x > 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && _transform->canLEFT && canDir("west",_transform))
                _transform->_velocity.x = -1;
            else if (_transform->_position.x + _entity->getComponent<SpriteComponent>().getTextureRect().width < 1920 
                    && sf::Keyboard::isKeyPressed(sf::Keyboard::D) && _transform->canRIGHT && canDir("east",_transform))
                _transform->_velocity.x = 1;
            else
                _transform->_velocity.x = 0;
        }
        
    private:
        Transform *_transform;
        sf::Event _event;
};

#endif /* !KEYINPUT_HPP_ */
