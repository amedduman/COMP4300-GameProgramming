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
        sf::Vertex lineA[] =
        {
            sf::Vertex(sf::Vector2f(100, 100)),
            sf::Vertex(sf::Vector2f(150, 150))
        };

        sf::Vertex lineB[] =
        {
            sf::Vertex(sf::Vector2f(100, 200)),
            sf::Vertex(sf::Vector2f(250, 250))
        };

        while (m_window.isOpen())
        {
            SInput();
            m_window.clear(sf::Color(25, 35,25));

            m_window.draw(lineA, 2, sf::Lines);
            m_window.draw(lineB, 2, sf::Lines);

            m_window.display();
        }
    }
private:
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
