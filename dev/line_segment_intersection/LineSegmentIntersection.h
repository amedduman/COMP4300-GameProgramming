#pragma once
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../../engine/ecs/EntityManager.h"


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

        auto b1 = sf::Vector2f(100, 200);
        auto b2 = sf::Vector2f(250, 250);
        sf::Vertex lineB[] =
        {
            sf::Vertex(b1),
            sf::Vertex(b2)
        };

        while (m_window.isOpen())
        {
            SInput();

            b2.x = sf::Mouse::getPosition(m_window).x;
            b2.y = sf::Mouse::getPosition(m_window).y;
            lineB[1] = sf::Vertex(sf::Vector2f(b2));

            auto intersectionPoint = LineIntersection(a1, a2, b1, b2);
            circle.setPosition(intersectionPoint);

            m_window.clear(sf::Color(25, 35,25));

            m_window.draw(lineA, 2, sf::Lines);
            m_window.draw(lineB, 2, sf::Lines);
            m_window.draw(circle);

            m_window.display();
        }
    }
private:
    sf::Vector2f LineIntersection(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
    {
        float a1 = p1.y - p0.y;
        float b1 = p0.x - p1.x;
        float c1 = a1 * p0.x + b1 * p0.y;

        float a2 = p3.y - p2.y;
        float b2 = p2.x - p3.x;
        float c2 = a2 * p2.x + b2 * p2.y;

        float denominator = a1 * b2 - a2 * b1;

        auto intersect = sf::Vector2f(0,0);
        intersect.x = (b2 * c1 - b1 * c2) / denominator;
        intersect.y = (a1 * c2 - a2 * c1) / denominator;

        return intersect;
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
