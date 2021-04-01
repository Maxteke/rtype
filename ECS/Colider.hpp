/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** Colider
*/

#ifndef COLIDER_HPP_
#define COLIDER_HPP_

#include "Components.hpp"
#include <functional>

class Colider : public Component {
    public:
        enum type {
            SOLID,
            OVERLAP
        };
        Colider(type type, std::function<void (Entity *)> overlap_callback = nullptr): _type(type), _overlap_callback(overlap_callback){}
        void init() override
        {
			if (_entity->hasComponent<SpriteComponent>()) {
	            _sprite = &_entity->getComponent<SpriteComponent>();
            	_colider = _sprite->getTextureRect();
			} else if (_entity->hasComponent<Button>()) {
				_colider = _entity->getComponent<Button>().getColider();
			}
			_transform = &_entity->getComponent<Transform>();
        }

        void update() override
        {
        }

		bool coliding(Entity *colid) {
            if (_transform->_position.x + _colider.width >= colid->getComponent<Transform>()._position.x &&
                colid->getComponent<Transform>()._position.x + colid->getComponent<Colider>().getColider().width >= _transform->_position.x &&
                _transform->_position.y + _colider.height >= colid->getComponent<Transform>()._position.y &&
                colid->getComponent<Transform>()._position.y + colid->getComponent<Colider>().getColider().height >= _transform->_position.y)
					return true;
			return false;
		}
		bool overlap_bis(int x1, int y1, int x2, int y2) {
			if (x1 <= y2 && y1 >= x2)
				return true;
			return false;
		}
		bool goingToColide(Entity *colid, bool solid) {
			if (_transform->_position.x - _transform->_speed <= colid->getComponent<Transform>()._position.x + colid->getComponent<Colider>().getColider().width
				&& _transform->_position.x >= colid->getComponent<Transform>()._position.x
				&& overlap_bis(_transform->_position.y, _transform->_position.y+_colider.height, colid->getComponent<Transform>()._position.y, colid->getComponent<Transform>()._position.y+colid->getComponent<Colider>().getColider().height)) {
				if (solid)
					_transform->canLEFT = false;
				return true;
			}
			if (_transform->_position.x + _colider.width + _transform->_speed >= colid->getComponent<Transform>()._position.x
				&& _transform->_position.x <= colid->getComponent<Transform>()._position.x
				&& overlap_bis(_transform->_position.y, _transform->_position.y+_colider.height, colid->getComponent<Transform>()._position.y, colid->getComponent<Transform>()._position.y+colid->getComponent<Colider>().getColider().height)) {
				if (solid)
					_transform->canRIGHT = false;
				return true;
			}
			if (_transform->_position.y - _transform->_speed <= colid->getComponent<Transform>()._position.y + colid->getComponent<Colider>().getColider().height
				&& _transform->_position.y >= colid->getComponent<Transform>()._position.y
				&& overlap_bis(_transform->_position.x, _transform->_position.x+_colider.width, colid->getComponent<Transform>()._position.x, colid->getComponent<Transform>()._position.x+colid->getComponent<Colider>().getColider().width)) {
				if (solid)
					_transform->canUP = false;
				return true;
			}
			if (_transform->_position.y + _colider.height + _transform->_speed >= colid->getComponent<Transform>()._position.y
				&& _transform->_position.y <= colid->getComponent<Transform>()._position.y
				&& overlap_bis(_transform->_position.x, _transform->_position.x+_colider.width, colid->getComponent<Transform>()._position.x, colid->getComponent<Transform>()._position.x+colid->getComponent<Colider>().getColider().width)) {
				if (solid)
					_transform->canDOWN = false;
				return true;
			}
			return false;
		}
        void isColidingWith(Entity *colid)
        {
			if (coliding(colid)) {
				if (colid->getComponent<Colider>().getType() == SOLID) {
					// std::cout << "THIS SHOULD NOT HAPPEN, MEANS SPRITE IS IN OTHER SPRITE" << std::endl;
				} else if (colid->getComponent<Colider>().getType() == OVERLAP) {
					_overlap_callback(colid);
				} else {
					// std::cout << "Handle other types than SOLID and OVERLAP" << std::endl;
				}
			} else {
				if (goingToColide(colid, false)) {
					if (colid->getComponent<Colider>().getType() == SOLID) {
						goingToColide(colid, true);
						if (std::find(_colid.begin(), _colid.end(), colid) == _colid.end())
							_colid.push_back(colid);
					}
				} else {
					if (std::find(_colid.begin(), _colid.end(), colid) != _colid.end())
						_colid.erase(std::find(_colid.begin(), _colid.end(), colid));
				}
			}
            if (_colid.size() == 0) {
                _transform->setFreeToMoove();
			}
        }

        sf::IntRect getColider()
        {
            return _colider;
        }

        type getType()
        {
            return _type;
        }

    private:
        SpriteComponent *_sprite;
        Transform *_transform;
        type _type;
        sf::IntRect _colider;
        std::vector<Entity *> _colid;
        std::function<void(Entity *)> _overlap_callback;
};

#endif /* !COLIDER_HPP_ */
