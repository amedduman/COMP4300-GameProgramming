#pragma once
#include "../math/Vec2.h"
#include <string>
#include <SFML/Graphics.hpp>

#include "Entity.h"

class Component
{
public:
    std::shared_ptr<Entity> entity;
    std::string cmpName;
    explicit Component(const std::shared_ptr<Entity>& _enity, const std::string& name)
        :entity(_enity),
         cmpName(name)
    {
    }
    virtual ~Component() = default;
};

class CTransform : public Component
{
public:
    Vec2 pos = Vec2(0,0);

    CTransform(const std::shared_ptr<Entity>& entity, const Vec2& p) : Component(entity, typeid(CTransform).name())
    {
        pos = p;
    }
};


class CShape : public Component
{
public:
    sf::RectangleShape rect;

    CShape(const std::shared_ptr<Entity>& entity, sf::Vector2f size, const sf::Color& fill) : Component(entity, typeid(CShape).name())
    {
        rect.setSize(size);
        rect.setFillColor(fill);
    }
};

// class CScore
// {
// public:
//     int score;

//     CScore(){}

//     CScore(int s)
//             : score(s)
//     {}
// };

// class CLifeSpan
// {
// public:
//     int remaining;
//     int total;

//     CLifeSpan(){}

//     CLifeSpan(int total)
//             : remaining(total), total(total)
//     {}
// };

// class CInput
// {
// public:
//     bool up =false;
//     bool down = false;
//     bool left = false;
//     bool right = false;
//     bool leftMouse = false;
//     bool rightMouse = false;
//     CInput() {}
// };

// class CCollision
// {
// public:
//     float radius;

//     CCollision(float r)
//             : radius (r){}
// };
