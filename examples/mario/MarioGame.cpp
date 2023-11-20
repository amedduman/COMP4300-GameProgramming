#include "MarioGame.h"
#include "../../engine/ecs/Components.h"

MarioGame::MarioGame()
    :m_window(sf::VideoMode(800, 600), "My window"),
     m_entityManager(EntityManager())
{}

void MarioGame::Run()
{
    SpawnPlayer();
    SpawnEnemies();

    while (m_window.isOpen())
    {
        m_entityManager.Update();

        SInput();
        SMovement();
        SRender();
    }
}

void MarioGame::SpawnPlayer()
{
    auto player = m_entityManager.AddEntity("player");
    m_entityManager.AddComponent(std::make_shared<CTransform>(player, Vec2(100, 100)));
    m_entityManager.AddComponent(std::make_shared<CShape>(player, sf::Vector2f(100, 100), sf::Color::White));
}

void MarioGame::SpawnEnemies()
{
    auto enemy = m_entityManager.AddEntity("enemy");
    m_entityManager.AddComponent(std::make_shared<CTransform>(enemy, Vec2(200, 100)));
    m_entityManager.AddComponent(std::make_shared<CShape>(enemy, sf::Vector2f(50, 50), sf::Color::Red));
}

void MarioGame::SInput()
{
    sf::Event event {};
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();
        if (event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::A)
            {
                printf("A pressed");
            }
        }
    }
}

void MarioGame::SMovement()
{
    for (auto& e : m_entityManager.GetEntities())
    {
        auto tr = m_entityManager.GetComponent<CTransform>(e);
        auto shape = m_entityManager.GetComponent<CShape>(e);
        if (tr && shape)
        {
            shape->rect.setPosition(tr->pos.x, tr->pos.y);
        }
    }
}
void MarioGame::SRender()
{
    m_window.clear(sf::Color::Black);
    for (auto& e : m_entityManager.GetComponents<CShape>())
    {
        m_window.draw(e->rect);
    }
    m_window.display();
}
