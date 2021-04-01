/*
** EPITECH PROJECT, 2020
** rtype
** File description:
** Button
*/

#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include "Components.hpp"

class Button : public Component {
    public:
        Button(sf::RenderWindow *win): _win(win){}

        void init() override
        {
			Transform tmp = _entity->getComponent<Transform>();
            _font.loadFromFile("../Assets/CGOTIC.ttf");
			_buttonText.setFont(_font);
			_buttonText.setString(_entity->getName());
			int buttonLength = _buttonText.getGlobalBounds().width + 4;
			int buttonHeight = 40;
			_colid.top = tmp._position.y;
			_colid.left = tmp._position.x;
			_colid.width = buttonLength;
			_colid.height = buttonHeight;
			_button.setSize(sf::Vector2f(buttonLength, buttonHeight));
			_button.setFillColor(sf::Color::Black);
			_button.setOutlineColor(sf::Color::Red);
			_button.setOutlineThickness(2);
			_button.setPosition(sf::Vector2f(tmp._position.x, tmp._position.y));
			_buttonText.setPosition(sf::Vector2f(tmp._position.x, tmp._position.y));
			_buttonText.setFillColor(sf::Color::Red);
        }

        void update() override
		{
		}

		void draw() override
		{
			_win->draw(_button);
			_win->draw(_buttonText);
		}

		sf::IntRect getColider() { return _colid; }
        
    private:
        sf::RectangleShape _button;
        sf::RenderWindow *_win;
        sf::Text _buttonText;
        sf::Font _font;
		sf::IntRect _colid;
};

#endif /* !BUTTON_HPP_ */
