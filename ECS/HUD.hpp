/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** HUD
*/

#ifndef HUD_HPP_
#define HUD_HPP_

#include "Components.hpp"

class HUD : public Component{
    public:
        HUD(sf::RenderWindow *win): _win(win){};
        void init() override
        {
            _font.loadFromFile("../Assets/CGOTIC.ttf");
            if (_entity->hasComponent<Health>()) {
                _health = &_entity->getComponent<Health>();
                _life.setPosition(sf::Vector2f(300, 930));
                _life.setFont(_font);
                _life.setFillColor(sf::Color::Red);
            }
            if (_entity->hasComponent<Weapon>()) {
                _weapon = &_entity->getComponent<Weapon>();
                _weapon_power.setPosition(sf::Vector2f(900, 930));
                _weapon_power.setFont(_font);
                _weapon_power.setFillColor(sf::Color::Green);
            }
        }

        void update() override
        {
            if (_entity->hasComponent<Health>())
                _life.setString("Life:  " + std::to_string(_health->getHP()));
            if (_entity->hasComponent<Weapon>())
                _weapon_power.setString("Weapon: " + _weapon->getName() + "    Power: " + std::to_string(_weapon->getPower()) + "    Ammo: " + std::to_string(_weapon->getAmmo()));
        }

        void draw() override
        {
            if (_entity->hasComponent<Health>())
                _win->draw(_life);
            if (_entity->hasComponent<Weapon>())
                _win->draw(_weapon_power);
        }
    private:
        Health *_health;
        Weapon *_weapon;
        sf::Text _life;
        sf::Text _weapon_power;
        sf::RenderWindow *_win;
        sf::Font _font;
};

#endif /* !HUD_HPP_ */
