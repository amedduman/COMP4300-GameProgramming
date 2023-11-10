#include "GeoWarsGame.h"
#include <iostream>


GeoWarsGame::GeoWarsGame()
    : m_entityManager(EntityManager()),
      m_window(sf::VideoMode(800, 600), "GeoWars")
{}

void GeoWarsGame::Run()
{
    m_window.setFramerateLimit(60);

    SpawnPlayer();
    while (m_window.isOpen())
    {
        m_entityManager.Update();
        SMovement();

        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();
        }
        m_window.clear(sf::Color::Black);
        for (auto& e : m_entityManager.GetEntities())
        {
            if (e->cShape)
            {
                m_window.draw(e->cShape->circle);
            }
        }
        m_window.display();
    }
}

void GeoWarsGame::SpawnPlayer()
{
    auto player = m_entityManager.AddEntity("player");
    player->cTransform = std::make_shared<CTransform>(CTransform(Vec2(100,100), Vec2(1,1), 3));
    player->cShape = std::make_shared<CShape>(CShape(32,12,sf::Color::Transparent,sf::Color::Red,3));
}

void GeoWarsGame::SMovement()
{
    for (auto& e : m_entityManager.GetEntities())
    {
        e->cTransform->pos.x += e->cTransform->velocity.x;
        e->cTransform->pos.y += e->cTransform->velocity.y;
        e->cShape->circle.setPosition(e->cTransform->pos.x,
                                      e->cTransform->pos.y);
    }
}

