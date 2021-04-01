/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** LifeBar
*/

#ifndef LIFEBAR_HPP_
#define LIFEBAR_HPP_

#include "Components.hpp"

class LifeBar : public Component {
    public:
        LifeBar(sf::RenderWindow *win): _win(win){}

        void init() override
        {
			_sprite = &_entity->getComponent<SpriteComponent>();
            _health = &_entity->getComponent<Health>();
			_life_bar.setFillColor(sf::Color(255, 35, 35, 150));
        }

        void update() override
		{
			_life_bar.setSize(sf::Vector2f(_health->getHP(), 5));
			_life_bar.setOrigin(_life_bar.getSize().x / 2, _life_bar.getSize().y / 2);
			_life_bar.setPosition(_entity->getComponent<Transform>()._position.x + _sprite->getTextureRect().width / 2, _entity->getComponent<Transform>()._position.y - 15);
		}

		void draw() override
		{
			_win->draw(_life_bar);
		}
        
    private:
        sf::RectangleShape _life_bar;
        Health *_health;
		SpriteComponent *_sprite;
        sf::RenderWindow *_win;

};

#endif /* !LIFEBAR_HPP_ */
