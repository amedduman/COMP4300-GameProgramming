#pragma once
#include "../math/Vec2.h"
#include <string>
#include <SFML/Graphics.hpp>

class Component
{
public:
    size_t ownerId = 0;
    std::string cmpName;
    Component(size_t id, std::string name)
    {
        ownerId = id;
        cmpName = name;
    }
};

class CTransform : Component
{
public:
    std::string cmpName = "transform";
    Vec2 pos = Vec2(0,0);
    Vec2 velocity = Vec2(0,0);

    CTransform(size_t id, const Vec2& p, const Vec2& s) : Component(id, cmpName)
    {
        pos = p;
    }
};


class CShape : Component
{
public:
    std::string cmpName = "shape";
    sf::CircleShape circle;

    CShape(size_t id, float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness) : Component(id, cmpName)
    {
        circle.setRadius(radius);
        circle.setPointCount(points);
        circle.setOutlineColor(outline);
        circle.setFillColor(fill);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
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
