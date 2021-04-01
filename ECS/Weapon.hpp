/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** Weapon
*/

#ifndef WEAPON_HPP_
#define WEAPON_HPP_

#include "Components.hpp"

class Weapon : public Component {
    public:
        Weapon(std::string texture = "../Assets/bullet1.png", std::string name = "piu piu", int power = 5, int bullet_speed = 2, int ammo = 100, int firerate = 200)
        :_texture(texture), _name(name), _power(power), _bullet_speed(bullet_speed), _ammo(ammo), _fireRate(firerate){}

        std::string getName()
        {
            return _name;
        }

        int getPower()
        {
            return _power;
        }

        int getSpeed()
        {
            return _bullet_speed;
        }

        int getAmmo()
        {
            return _ammo;
        }

        int getFireRate()
        {
            return _fireRate;
        }

        std::string getTexture()
        {
            return _texture;
        }
        void useAmmo()
        {
            _ammo--;
        }
        void setWeapon(std::string texture, std::string name, int power, int bullet_speed, int ammo, int firerate) {
            _texture = texture;
            _name = name;
            _power = power;
            _bullet_speed = bullet_speed;
            _ammo = ammo;
            _fireRate = firerate;
        }
    private:
        std::string _texture;
        std::string _name;
        int _power;
        int _bullet_speed;
        int _ammo;
		int _fireRate;

};

#endif /* !WEAPON_HPP_ */
