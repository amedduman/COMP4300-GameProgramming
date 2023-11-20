#include "MarioGame.h"
#include "../../engine/ecs/Components.h"

MarioGame::MarioGame()
    :m_window(sf::VideoMode(800, 600), "My window"),
     m_entityManager(EntityManager())
{
    m_window.setFramerateLimit(60);
}

void MarioGame::Run()
{
    SpawnPlayer();
    SpawnEnemies();

    while (m_window.isOpen())
    {
        m_entityManager.Update();

        SInput();
        SCalculateVelocity();
        SMovement();
        SDetectCollision();
        SRender();
    }
}

void MarioGame::SpawnPlayer()
{
    auto player = m_entityManager.AddEntity("player");
    m_entityManager.AddComponent(std::make_shared<CTransform>(player, Vec2(100, 100)));
    m_entityManager.AddComponent(std::make_shared<CShape>(player, sf::Vector2f(25, 25), sf::Color::White));
    m_entityManager.AddComponent(std::make_shared<CMarioInput>(player));
    m_entityManager.AddComponent(std::make_shared<CVelocity>(player));
}
void MarioGame::SpawnEnemies()
{
    auto enemy = m_entityManager.AddEntity("enemy");
    m_entityManager.AddComponent(std::make_shared<CTransform>(enemy, Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2)));
    m_entityManager.AddComponent(std::make_shared<CShape>(enemy, sf::Vector2f(200, 200), sf::Color::Transparent, sf::Color::Yellow, 4));
    m_entityManager.AddComponent(std::make_shared<CVelocity>(enemy));
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
            const auto input = m_entityManager.GetComponent<CMarioInput>("player");
            if(event.key.code == sf::Keyboard::W)
            {
                input->moveUP = true;
            }
            if(event.key.code == sf::Keyboard::A)
            {
                input->moveLeft = true;
            }
            if(event.key.code == sf::Keyboard::S)
            {
                input->moveDown = true;
            }
            if(event.key.code == sf::Keyboard::D)
            {
                input->moveRight = true;
            }

            if(event.key.code == sf::Keyboard::Escape)
            {
                m_window.close();
            }
        }
        if (event.type == sf::Event::KeyReleased)
        {
            const auto input = m_entityManager.GetComponent<CMarioInput>("player");
            if(event.key.code == sf::Keyboard::W)
            {
                input->moveUP = false;
            }
            if(event.key.code == sf::Keyboard::A)
            {
                input->moveLeft = false;
            }
            if(event.key.code == sf::Keyboard::S)
            {
                input->moveDown = false;
            }
            if(event.key.code == sf::Keyboard::D)
            {
                input->moveRight = false;
            }
        }
    }
}
void MarioGame::SCalculateVelocity()
{
    auto playerVel = m_entityManager.GetComponent<CVelocity>("player");
    auto playerInp = m_entityManager.GetComponent<CMarioInput>("player");
    playerVel->velocity = Vec2(0,0);
    constexpr float speed = 10;
    if (playerInp->moveUP) playerVel->velocity.y = -speed;
    if (playerInp->moveDown) playerVel->velocity.y = speed;
    if (playerInp->moveRight) playerVel->velocity.x = speed;
    if (playerInp->moveLeft) playerVel->velocity.x = -speed;
}
void MarioGame::SMovement()
{
    for (auto& e : m_entityManager.GetEntities())
    {
        auto tr = m_entityManager.GetComponent<CTransform>(e);
        auto shape = m_entityManager.GetComponent<CShape>(e);
        auto vel = m_entityManager.GetComponent<CVelocity>(e);
        if (tr && shape && vel)
        {
            tr->pos.x += vel->velocity.x;
            tr->pos.y += vel->velocity.y;
            shape->rect.setPosition(tr->pos.x, tr->pos.y);
        }
    }
}
void MarioGame::SDetectCollision()
{
    // log if player is inside enemy
    auto enemyShape = m_entityManager.GetComponent<CShape>("enemy");
    auto enemyTr = m_entityManager.GetComponent<CTransform>("enemy");
    Vec2 rectPos = Vec2(enemyTr->pos.x,
                        enemyTr->pos.y);
    Vec2 rectPosTopRight = Vec2(enemyTr->pos.x - enemyShape->rect.getSize().x / 2,
                                enemyTr->pos.y - enemyShape->rect.getSize().y / 2);
    Vec2 rectPosLeftBottom = Vec2(enemyTr->pos.x + enemyShape->rect.getSize().x / 2,
                                enemyTr->pos.y + enemyShape->rect.getSize().y / 2);
    AddDebugShape(sf::Vector2f(rectPosTopRight.x,rectPosTopRight.y), sf::Vector2f(30,10));
    AddDebugShape(sf::Vector2f(rectPosLeftBottom.x,rectPosLeftBottom.y), sf::Vector2f(30,10));

    auto playerTr = m_entityManager.GetComponent<CTransform>("player");
    if (rectPosTopRight.x < playerTr->pos.x &&
        rectPosLeftBottom.x > playerTr->pos.x &&
        rectPosTopRight.y < playerTr->pos.y &&
        rectPosLeftBottom.y > playerTr->pos.y)
    {
        enemyShape->rect.setOutlineColor(sf::Color::Green);
    }
    else
    {
        enemyShape->rect.setOutlineColor(sf::Color::Yellow);
    }
}
void MarioGame::SRender()
{
    m_window.clear(sf::Color::Black);
    for (auto& e : m_entityManager.GetComponents<CShape>())
    {
        m_window.draw(e->rect);
    }
    for (auto& e : m_debugShapes)
    {
        m_window.draw(e);
    }
    m_window.display();
}

void MarioGame::AddDebugShape(sf::Vector2f pos, sf::Vector2f size, sf::Color fCol, sf::Color oCol, float thickness)
{
    auto r = sf::RectangleShape();
    r.setSize(size);
    r.setOrigin(r.getSize().x/2,r.getSize().y/2);
    r.setPosition(pos);
    r.setFillColor(fCol);
    r.setOutlineColor(oCol);
    r.setOutlineThickness(thickness);
    m_debugShapes.push_back(r);
}
