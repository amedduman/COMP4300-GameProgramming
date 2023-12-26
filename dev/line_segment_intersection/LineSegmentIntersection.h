#pragma once
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../../engine/ecs/EntityManager.h"

struct Intersect
{
    Intersect()
    {
        isIntersected = false;
        intersectionPoint = sf::Vector2f(0,0);
    }
    bool isIntersected;
    sf::Vector2f intersectionPoint;
};
struct Line
{
    sf::Vector2f p0 = sf::Vector2f(0,0);
    sf::Vector2f p1 = sf::Vector2f(0,0);
};
struct Circle
{
    sf::CircleShape shape;
    Circle()
    {
        shape = sf::CircleShape(4);
        shape.setOrigin(4,4);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(1);
    }
};

void SpawnRects(const sf::RenderWindow& window, std::vector<sf::RectangleShape>& rects)
{
    sf::RectangleShape rect1;
    rect1.setSize(sf::Vector2f(100,100));
    rect1.setPosition(500,300);
    rect1.setOutlineThickness(1);
    rect1.setFillColor(sf::Color::Transparent);

    sf::RectangleShape rect2;
    rect2.setSize(sf::Vector2f(100,100));
    rect2.setPosition(100,300);
    rect2.setOutlineThickness(1);
    rect2.setFillColor(sf::Color::Transparent);

    sf::RectangleShape screenBounds;
    screenBounds.setSize(sf::Vector2f(window.getSize().x,window.getSize().y));
    screenBounds.setPosition(0,0);
    screenBounds.setOutlineThickness(1);
    screenBounds.setFillColor(sf::Color::Transparent);

    rects.push_back(rect1);
    rects.push_back(rect2);
    rects.push_back(screenBounds);
}

class LineSegmentIntersection
{
public:
    sf::RenderWindow m_window;
    EntityManager m_entityManager;

    LineSegmentIntersection()
        :m_window(sf::VideoMode(800, 600), "My window")
    {}
    void Run()
    {
        // rects
        std::vector<sf::RectangleShape> rects;
        SpawnRects(m_window, rects);

        while (m_window.isOpen())
        {
            std::vector<Circle> circles;
            std::vector<Line> lines;

            SInput();

            for(auto& e : rects)
            {
                for (int i = 0; i < e.getPointCount(); ++i)
                {
                    Line line;
                    line.p0.x = sf::Mouse::getPosition(m_window).x;
                    line.p0.y = sf::Mouse::getPosition(m_window).y;
                    line.p1 += e.getPoint(i) + e.getPosition();
                    auto dir = line.p1 - line.p0;
                    line.p1 += sf::Vector2f(dir.x * 100, dir.y * 100);
                    lines.push_back(line);

                    Line l2;
                    l2.p0 = line.p0;

                    float r1 = 0.001;
                    l2.p1.x = line.p1.x * cos(r1) - line.p1.y * sin(r1);
                    l2.p1.y = line.p1.x * sin(r1) + line.p1.y * cos(r1);

                    lines.push_back(l2);

                    Line l3;
                    l3.p0 = line.p0;

                    float r2 = -r1;
                    l3.p1.x = line.p1.x * cos(r2) - line.p1.y * sin(r2);
                    l3.p1.y = line.p1.x * sin(r2) + line.p1.y * cos(r2);

                    lines.push_back(l3);
                }
            }

            for(auto& l : lines)
            {
                Intersect intersectionResult;
                for(auto& e : rects)
                {
                    intersectionResult = LineVsRect(l.p0,l.p1, e);
                    if (intersectionResult.isIntersected) break;
                }
                if (intersectionResult.isIntersected)
                {
                    auto c = Circle();
                    c.shape.setPosition(intersectionResult.intersectionPoint);
                    circles.push_back(c);
                }
            }

            m_window.clear(sf::Color(25, 35,25));

            for(auto& e : rects)
            {
                m_window.draw(e);
            }
            for(auto& e : lines)
            {
                sf::Vertex line[] =
                {
                    sf::Vertex(e.p0),
                    sf::Vertex(e.p1)
                };
                m_window.draw(line, 2, sf::Lines);
            }
            for(auto& c : circles)
            {
                m_window.draw(c.shape);
            }

            m_window.display();
        }
    }
private:
    Intersect LineVsRect(sf::Vector2f p0, sf::Vector2f p1, sf::RectangleShape rect)
    {
        Intersect result;
        result.isIntersected = false;
        result.intersectionPoint = sf::Vector2f(0,0);
        sf::Vector2f a = sf::Vector2f(rect.getPosition().x + rect.getPoint(0).x, rect.getPosition().y + rect.getPoint(0).y);
        sf::Vector2f b= sf::Vector2f(rect.getPosition().x + rect.getPoint(1).x, rect.getPosition().y + rect.getPoint(1).y);
        sf::Vector2f c= sf::Vector2f(rect.getPosition().x + rect.getPoint(2).x, rect.getPosition().y + rect.getPoint(2).y);
        sf::Vector2f d= sf::Vector2f(rect.getPosition().x + rect.getPoint(3).x, rect.getPosition().y + rect.getPoint(3).y);

        // ab bc cd da
        std::vector<sf::Vector2f> intersections;
        auto i1 = LineSegIntersect(p0, p1, a, b);
        auto i2 = LineSegIntersect(p0, p1, b, c);
        auto i3 = LineSegIntersect(p0, p1, c, d);
        auto i4 = LineSegIntersect(p0, p1, d, a);
        if(i1.isIntersected) intersections.push_back(i1.intersectionPoint);
        if(i2.isIntersected) intersections.push_back(i2.intersectionPoint);
        if(i3.isIntersected) intersections.push_back(i3.intersectionPoint);
        if(i4.isIntersected) intersections.push_back(i4.intersectionPoint);

        float distanceSqr = 1000000;
        for(auto& e: intersections)
        {
            auto vec = sf::Vector2f(e.x - p0.x, e.y - p0.y);
            float vecSqr = vec.x * vec.x + vec.y * vec.y;
            if(vecSqr < distanceSqr)
            {
                distanceSqr = vecSqr;
                result.isIntersected = true;
                result.intersectionPoint = e;
            }
        }
        return result;
    }

    Intersect LineSegIntersect(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d)
    {
        Intersect result;

        Vec2 r = Vec2(b.x - a.x, b.y - a.y);
        Vec2 s = Vec2(d.x - c.x, d.y - c.y);
        float rxs = Cross(r, s);
        Vec2 cma = Vec2(c.x - a.x, c.y - a.y);

        float t = Cross(cma, s) / rxs;
        float u = Cross(cma, r) / rxs;

        if(t >= 0 && t <= 1 && u >= 0 && u <= 1)
        {
            result.isIntersected = true;
            result.intersectionPoint = sf::Vector2f(a.x + t*r.x, a.y + t*r.y);
        }
        else
        {
            result.isIntersected = false;
            result.intersectionPoint = sf::Vector2f(0,0);
        }

        return result;
    }

    float Cross(Vec2 a, Vec2 b)
    {
        return a.x * b.y - a.y * b.x;
    }

    void SInput()
    {
        sf::Event event {};
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    m_window.close();
                }
            }
            if (event.type == sf::Event::KeyReleased)
            {

            }
        }
    }
};
