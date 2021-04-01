/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** SpriteComponent
*/

#ifndef SPRITECOMPONENT_HPP_
#define SPRITECOMPONENT_HPP_

#include "Components.hpp"
#include "SFML/Graphics.hpp"

class SpriteComponent : public Component {
    public:
        SpriteComponent(std::string path, sf::RenderWindow *win): _win(win)
        {
            setTexture(path);
        }
        void init() override
        {
            if (!_entity->hasComponent<Transform>())
                _entity->addComponent<Transform>();
            _transform = &_entity->getComponent<Transform>();
            _sprite.setTexture(_texture);
            _sprite.setPosition(_transform->_position.x, _transform->_position.y);
        }
        void update() override
        {
            _sprite.setPosition(_transform->_position.x, _transform->_position.y);
        }
        void draw() override
        {
            _win->draw(_sprite);
        }
        void setTexture(std::string path)
        {
            if (!_texture.loadFromFile(path))
                std::cout << "SpriteComponent" << std::endl;
        }

        void setRotation(int rot)
        {
            _sprite.setRotation(rot);
        }

        sf::IntRect getTextureRect()
        {
            return _sprite.getTextureRect();
        }
    private:
        Transform *_transform;
        sf::Texture _texture;
        sf::Sprite _sprite;
        sf::RenderWindow *_win;
};

#endif /* !SPRITECOMPONENT_HPP_ */
