/*
** EPITECH PROJECT, 2020
** B-CPP-501-LYN-5-1-rtype-maxence.pellerin
** File description:
** ECS
*/

#ifndef ECS_HPP_
#define ECS_HPP_

#include <iostream>
#include <bitset>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponent = 32;

using ComponentBitSet = std::bitset<maxComponent>;
using ComponentArray = std::array<Component *, maxComponent>;

class Component {
    public:
        Entity *_entity;
        virtual void init(){}
        virtual void update(){}
        virtual void draw(){}
        virtual ~Component(){}
};

class Entity {
    public:
        Entity(std::string type, std::string name, Entity *parent):_type(type), _name(name), _parent(parent){}
        void update()
        {
            for (auto &component: _components)
                component->update();
        }
        void draw()
        {
            for (auto &component: _components)
                component->draw();
        }
        bool isActive() const { return _active; }
        void destroy() { _active = false; }

        template <typename T> bool hasComponent() const
        {
            return _componentBitSet[getComponentTypeID<T>()];
        }

        template <typename T, typename... TArgs> T &addComponent(TArgs&&... mArgs)
        {
            T *component(new T(std::forward<TArgs>(mArgs)...));
            component->_entity = this;
            std::unique_ptr<Component> uPtr( component );
            _components.emplace_back(std::move(uPtr));
            _componentArray[getComponentTypeID<T>()] = component;
            _componentBitSet[getComponentTypeID<T>()] = true;
            component->init();
            return *component;
        }

        template <typename T> T &getComponent() const
        {
            auto ptr(_componentArray[getComponentTypeID<T>()]);
            return *static_cast<T*>(ptr);
        }

        std::string getType()
        {
            return _type;
        }

        std::string getName()
        {
            return _name;
        }

        Entity *getParent()
        {
            return _parent;
        }
    private:
        bool _active = true;
        std::vector<std::unique_ptr<Component>> _components;
        ComponentArray _componentArray;
        ComponentBitSet _componentBitSet;
        std::string _type;
        std::string _name;
        Entity *_parent;
};

class EntityManager
{
    public:
        void update()
        {
            for (auto &entity: _entities)
                entity->update();
        }

        void draw()
        {
            for (auto &entity: _entities)
                entity->draw();
        }

        void refresh()
        {
            _entities.erase(std::remove_if(std::begin(_entities), std::end(_entities),
                [](const Entity *mEntity)
                {
                    return !mEntity->isActive();   
                }), std::end(_entities));
        }

        Entity &addEntity(std::string type, std::string name, Entity *parent = nullptr)
        {
            Entity *entity = new Entity(type, name, parent);
            _entities.emplace_back(entity);
            return *entity;
        }

        std::vector<Entity *> getEntities()
        {
            return _entities;
        }
        
    private:
        std::vector<Entity *> _entities;
};

#endif /* !ECS_HPP_ */
