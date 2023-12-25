#pragma once
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../../engine/ecs/EntityManager.h"

struct LineIntersectionResult
{
    bool isIntersected;
    sf::Vector2f intersectionPoint;
};

class LineSegmentIntersection
{
public:
    sf::RenderWindow m_window;
    EntityManager m_entityManager;

    LineSegmentIntersection()
        :m_window(sf::VideoMode(800, 600), "My window")
    {
    }
    void Run()
    {
        auto circle = sf::CircleShape(4);
        circle.setOrigin(4,4);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(1);

        auto a1 = sf::Vector2f(400, 500);
        auto a2 = sf::Vector2f(550, 150);
        sf::Vertex lineA[] =
        {
            sf::Vertex(a1),
            sf::Vertex(a2)
        };
        lineA[0].color = sf::Color::Red;
        lineA[1].color = sf::Color::Red;

        auto b1 = sf::Vector2f(400, 600);
        auto b2 = sf::Vector2f(550, 250);
        sf::Vertex lineB[] =
        {
            sf::Vertex(b1),
            sf::Vertex(b2)
        };

        sf::RectangleShape rect1;
        rect1.setSize(sf::Vector2f(100,100));
        rect1.setPosition(500,300);
        rect1.setOutlineThickness(1);
        rect1.setFillColor(sf::Color::Transparent);

        sf::RectangleShape rect2;
        rect2.setSize(sf::Vector2f(300,100));
        rect2.setPosition(300,300);
        rect2.setOutlineThickness(1);
        rect2.setFillColor(sf::Color::Transparent);

        /*
        sf::Vector2f a = sf::Vector2f(rect.getPosition().x + rect.getPoint(0).x, rect.getPosition().y + rect.getPoint(0).y);
        sf::Vector2f b= sf::Vector2f(rect.getPosition().x + rect.getPoint(1).x, rect.getPosition().y + rect.getPoint(1).y);
        sf::Vector2f c= sf::Vector2f(rect.getPosition().x + rect.getPoint(2).x, rect.getPosition().y + rect.getPoint(2).y);
        sf::Vector2f d= sf::Vector2f(rect.getPosition().x + rect.getPoint(3).x, rect.getPosition().y + rect.getPoint(3).y);
        // std::cout << a.x << ", " << a.y << std::endl;
        // std::cout << b.x << ", " << b.y << std::endl;
        // std::cout << c.x << ", " << c.y << std::endl;
        // std::cout << d.x << ", " << d.y << std::endl;
        sf::Vertex ab[] =
        {
            sf::Vertex(a),
            sf::Vertex(b),
        };
        sf::Vertex bc[] =
        {
            sf::Vertex(b),
            sf::Vertex(c),
        };
        sf::Vertex cd[] =
        {
            sf::Vertex(c),
            sf::Vertex(d),
        };
        sf::Vertex da[] =
        {
            sf::Vertex(d),
            sf::Vertex(a),
        };
        */

        while (m_window.isOpen())
        {
            SInput();

            b2.x = sf::Mouse::getPosition(m_window).x;
            b2.y = sf::Mouse::getPosition(m_window).y;
            lineB[1] = sf::Vertex(sf::Vector2f(b2));

            // auto intersectionResult = LineSegIntersect(a1, a2, b1, b2);
            auto intersectionResult = LineVsRect(b1,b2, rect1);
            if (intersectionResult.isIntersected)
            {
                circle.setPosition(intersectionResult.intersectionPoint);
            }
            else
                circle.setPosition(sf::Vector2f(0,0));

            // LineVsRect(b1,b2, rect);

            m_window.clear(sf::Color(25, 35,25));

            m_window.draw(lineA, 2, sf::Lines);
            m_window.draw(lineB, 2, sf::Lines);
            m_window.draw(circle);
            m_window.draw(rect1);
            // m_window.draw(ab, 4, sf::Lines);
            // m_window.draw(bc, 4, sf::Lines);
            // m_window.draw(cd, 4, sf::Lines);
            // m_window.draw(da, 4, sf::Lines);

            m_window.display();
        }
    }
private:
    LineIntersectionResult LineSegIntersect(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d)
    {
        LineIntersectionResult result;

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
    LineIntersectionResult LineIntersection(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
    {
        LineIntersectionResult result;

        float a1 = p1.y - p0.y;
        float b1 = p0.x - p1.x;
        float c1 = a1 * p0.x + b1 * p0.y;

        float a2 = p3.y - p2.y;
        float b2 = p2.x - p3.x;
        float c2 = a2 * p2.x + b2 * p2.y;

        float denominator = a1 * b2 - a2 * b1;
        if(denominator == 0)
        {
            result.isIntersected = false;

            result.intersectionPoint = sf::Vector2f(0,0);
        }
        else
        {
            result.isIntersected = true;
            auto intersect = sf::Vector2f(0,0);
            intersect.x = (b2 * c1 - b1 * c2) / denominator;
            intersect.y = (a1 * c2 - a2 * c1) / denominator;

            result.intersectionPoint = intersect;
        }

        return result;
    }
    LineIntersectionResult LineVsRect(sf::Vector2f p0, sf::Vector2f p1, sf::RectangleShape rect)
    {
        LineIntersectionResult result;
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
    void SRender()
    {
        m_window.clear(sf::Color(25, 35,25));
        for (auto& e : m_entityManager.GetComponents<CShape>())
        {
            m_window.draw(e->rect);
        }
        for (auto& e : m_entityManager.GetComponents<CSprite>())
        {
            m_window.draw(e->GetSprite());
        }
        m_window.display();
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
