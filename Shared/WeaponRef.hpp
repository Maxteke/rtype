/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** WeaponRef
*/

#ifndef WEAPONREF_HPP_
#define WEAPONREF_HPP_

#include <iostream>

class weapon {
	public:
		weapon(const std::string &n)
		{
			_name = n;
			_text = "";
			_power = _speed = _ammo = 0;
			if (n == "WEP_basic") {
				_text = "../Assets/bullet1.png";
				_power = 5;
				_speed = 10;
				_ammo = 100;
				_fireRate = 200;
			} else if ( n == "WEP_normal") {
				_power = 2;
				_speed = 20;
				_ammo = 1000;
				_fireRate = 100;
				_text = "../Assets/bullet2.png";
			} else {
				std::cout << "This weapon does not exist. FIX. Game.hpp." << std::endl;
			}
		};

		std::string _name;
		std::string _text;
		int _power;
		int _speed;
		int _ammo;
		int _fireRate;
};

#endif /* !WEAPONREF_HPP_ */
