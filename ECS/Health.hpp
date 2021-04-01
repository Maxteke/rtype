/*
** EPITECH PROJECT, 2020
** rtype
** File description:
** Health
*/

#ifndef HEALTH_HPP_
#define HEALTH_HPP_
#include "Components.hpp"

class Health : public Component {
	public:
		Health(int health): _max_health(health){};
		void init() override
		{
			_health = _max_health;
		}
		int getHP() { return _health; };
		void setHP(int hp) { if (hp > _max_health) { _health = 100;} else if (hp <= 0) {_health = 0; _entity->destroy();} else { _health = hp;}; };

	private:
		int _health;
		int _max_health;
};

#endif /* !HEALTH_HPP_ */
