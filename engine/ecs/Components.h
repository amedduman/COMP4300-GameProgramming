#pragma once
#include "../math/Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform
{
public:
    Vec2 pos = Vec2(0,0);
    Vec2 velocity = Vec2(0,0);
    float angle = 0;

    CTransform(){}

    CTransform(const Vec2& p, const Vec2& s,float a)
            : pos(p),
              velocity(s),
              angle(a)
    {}
};


class CShape
{
public:
    sf::CircleShape circle;

    CShape(){}

    CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
            : circle(radius, points)
    {
        circle.setOutlineColor(outline);
        circle.setFillColor(fill);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
    }

};

class CScore
{
public:
    int score;

    CScore(){}

    CScore(int s)
            : score(s)
    {}
};

class CLifeSpan
{
public:
    int remaining;
    int total;

    CLifeSpan(){}

    CLifeSpan(int total)
            : remaining(total), total(total)
    {}
};

class CInput
{
public:
    bool up =false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool leftMouse = false;
    bool rightMouse = false;
    CInput() {}
};

class CCollision
{
public:
    float radius;

    CCollision(float r)
            : radius (r){}
};
