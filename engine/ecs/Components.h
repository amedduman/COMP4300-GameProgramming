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

    CShape(const std::shared_ptr<Entity>& entity, sf::Vector2f size, const sf::Color& fill , const sf::Color& outline = sf::Color::White, float thickness = 0) : Component(entity, typeid(CShape).name())
    {
        rect.setSize(size);
        rect.setFillColor(fill);
        rect.setOrigin(size.x/2, size.y/2);
        rect.setOutlineThickness(thickness);
        rect.setOutlineColor(outline);
    }
};

class CBoxCollider : public Component
{
public:
    float width;
    float height;
    CBoxCollider(const std::shared_ptr<Entity>& entity, const std::shared_ptr<CShape>& shape) : Component(entity, typeid(CBoxCollider).name())
    {
        m_shape = shape;
        this->width = shape->rect.getSize().x;
        this->height = shape->rect.getSize().y;
    }

    Vec2 TopLeft()
    {
        auto topLeft = Vec2(m_shape->rect.getPosition().x - m_shape->rect.getSize().x / 2,
                       m_shape->rect.getPosition().y - m_shape->rect.getSize().y / 2);
        return topLeft;
    }
private:
    std::shared_ptr<CShape> m_shape;
};

class CMarioInput : public Component
{
public:
    CMarioInput(const std::shared_ptr<Entity>& entity) : Component(entity, typeid(CMarioInput).name())
    {}

    bool moveUP = false;
    bool moveDown = false;
    bool moveRight = false;
    bool moveLeft = false;
    bool fire = false;
};

class CVelocity : public Component
{
public:
    Vec2 velocity;
    explicit CVelocity(const std::shared_ptr<Entity>& entity) : Component(entity, typeid(CVelocity).name())
    {
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
