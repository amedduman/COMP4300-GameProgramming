#include "GeoWarsGame.h"
#include <iostream>


GeoWarsGame::GeoWarsGame()
    : m_entityManager(EntityManager()),
      m_window(sf::VideoMode(800, 600), "GeoWars")
{

}

void GeoWarsGame::Run()
{
    m_window.setFramerateLimit(60);
    m_player =  SpawnPlayer();

    while (m_window.isOpen())
    {
        m_entityManager.Update();
        SUserInput();
        SMovement();
        SRender();
    }
}

std::shared_ptr<Entity> GeoWarsGame::SpawnPlayer()
{
    auto player = m_entityManager.AddEntity("player");
    player->cTransform = std::make_shared<CTransform>(CTransform(Vec2(100,100), Vec2(1,1), 3));
    player->cShape = std::make_shared<CShape>(CShape(32,12,sf::Color::Transparent,sf::Color::Red,3));
    player->cInput = std::make_shared<CInput>();
    return player;
}

void GeoWarsGame::SUserInput()
{
    m_player->cInput->left = false;
    m_player->cInput->right = false;
    m_player->cInput->up = false;
    m_player->cInput->down = false;

    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();

        if(event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::A:
                    m_player->cInput->left = true;
                    printf("a");
                    break;
                case sf::Keyboard::D:
                    m_player->cInput->right = true;
                    printf("d");
                    break;
                case sf::Keyboard::W:
                    m_player->cInput->up = true;
                    break;
                case sf::Keyboard::S:
                    m_player->cInput->down = true;
                    break;
                case sf::Keyboard::Escape:
                    m_window.close();
                    break;
            }
        }
    }
}

void GeoWarsGame::SMovement()
{
    m_player->cTransform->velocity = Vec2(0,0);
    float speed = 8;
    if (m_player->cInput->right)
        m_player->cTransform->velocity.x = 1 * speed;
    if (m_player->cInput->left)
        m_player->cTransform->velocity.x = -1 * speed;
    if (m_player->cInput->up)
        m_player->cTransform->velocity.y = -1 * speed;
    if (m_player->cInput->down)
        m_player->cTransform->velocity.y = 1 * speed;

    for (auto& e : m_entityManager.GetEntities())
    {
        e->cTransform->pos.x += e->cTransform->velocity.x;
        e->cTransform->pos.y += e->cTransform->velocity.y;
        e->cShape->circle.setPosition(e->cTransform->pos.x,
                                      e->cTransform->pos.y);
    }
}

void GeoWarsGame::SRender()
{
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

