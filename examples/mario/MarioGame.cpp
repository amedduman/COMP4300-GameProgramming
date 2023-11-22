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
    m_entityManager.AddComponent(std::make_shared<CBoxCollider>(player, m_entityManager.GetComponent<CShape>(player)));
}
void MarioGame::SpawnEnemies()
{
    auto enemy = m_entityManager.AddEntity("enemy");
    m_entityManager.AddComponent(std::make_shared<CTransform>(enemy, Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2)));
    m_entityManager.AddComponent(std::make_shared<CShape>(enemy, sf::Vector2f(200, 200), sf::Color::Transparent, sf::Color::Yellow, 4));
    m_entityManager.AddComponent(std::make_shared<CVelocity>(enemy));
    m_entityManager.AddComponent(std::make_shared<CBoxCollider>(enemy, m_entityManager.GetComponent<CShape>(enemy)));
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
    auto rectE = m_entityManager.GetComponent<CBoxCollider>("enemy");
    auto rectP = m_entityManager.GetComponent<CBoxCollider>("player");
    auto point = m_entityManager.GetComponent<CTransform>("player")->pos;
    // if (PointVsRect(point, rectE))
    //     m_entityManager.GetComponent<CShape>("enemy")->rect.setOutlineColor(sf::Color::Green);
    // else
    //     m_entityManager.GetComponent<CShape>("enemy")->rect.setOutlineColor(sf::Color::Yellow);
    if (RectVsRect(rectE, rectP))
        m_entityManager.GetComponent<CShape>("enemy")->rect.setOutlineColor(sf::Color::Green);
    else
        m_entityManager.GetComponent<CShape>("enemy")->rect.setOutlineColor(sf::Color::Yellow);
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
        m_window.draw(e->rect);
    }
    m_window.display();
}

void MarioGame::AddDebugShape(const std::shared_ptr<CShape>& shape)
{
    // shape->rect.setFillColor(sf::Color::Transparent);
    // shape->rect.setOutlineColor(sf::Color::Green);
    m_debugShapes.push_back(shape);
}

bool MarioGame::PointVsRect(Vec2 point, const std::shared_ptr<CBoxCollider>& rect)
{
    if (point.x > rect->TopLeft().x                &&
        point.x < rect->TopLeft().x + rect->width  &&
        point.y > rect->TopLeft().y                &&
        point.y < rect->TopLeft().y + rect->height)
    {
        return true;
    }
    return false;
}

bool MarioGame::RectVsRect(const std::shared_ptr<CBoxCollider>& rect1, const std::shared_ptr<CBoxCollider>& rect2)
{
    if(rect1->TopLeft().x  < rect2->TopLeft().x + rect2->width  &&
        rect2->TopLeft().x < rect1->TopLeft().x + rect1->width  &&
        rect1->TopLeft().y < rect2->TopLeft().y + rect2->height &&
        rect2->TopLeft().y < rect1->TopLeft().y + rect1->height )
    {
        return true;
    }
    return false;
}
