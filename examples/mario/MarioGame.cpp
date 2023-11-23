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
    constexpr float sizeX = 25;
    constexpr float sizeY = 25;
    m_entityManager.AddComponent(std::make_shared<CTransform>(player, Vec2(100, 100)));
    m_entityManager.AddComponent(std::make_shared<CShape>(player, sf::Vector2f(sizeX, sizeY), sf::Color::Transparent));
    m_entityManager.AddComponent(std::make_shared<CMarioInput>(player));
    m_entityManager.AddComponent(std::make_shared<CVelocity>(player));
    m_entityManager.AddComponent(std::make_shared<CBoundingBox>(player, Vec2(sizeX, sizeY)));
}
void MarioGame::SpawnEnemies()
{
    auto enemy = m_entityManager.AddEntity("enemy");
    constexpr float sizeX = 200;
    constexpr float sizeY = 200;
    m_entityManager.AddComponent(std::make_shared<CTransform>(enemy, Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2)));
    m_entityManager.AddComponent(std::make_shared<CShape>(enemy, sf::Vector2f(sizeX, sizeY), sf::Color::Transparent, sf::Color::Yellow, 1));
    m_entityManager.AddComponent(std::make_shared<CVelocity>(enemy));
    m_entityManager.AddComponent(std::make_shared<CBoundingBox>(enemy, Vec2(sizeY, sizeY)));
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
    auto rectE = m_entityManager.GetComponent<CBoundingBox>("enemy");
    auto rectP = m_entityManager.GetComponent<CBoundingBox>("player");

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

bool MarioGame::PointVsRect(Vec2 point, const std::shared_ptr<CBoundingBox>& rect)
{
    // if (point.x > rect->TopLeft().x                &&
    //     point.x < rect->TopLeft().x + rect->width  &&
    //     point.y > rect->TopLeft().y                &&
    //     point.y < rect->TopLeft().y + rect->height)
    // {
    //     return true;
    // }
    // return false;
}

bool MarioGame::RectVsRect(const std::shared_ptr<CBoundingBox>& bb1, const std::shared_ptr<CBoundingBox>& bb2)
{
    auto bb1pos = m_entityManager.GetComponent<CTransform>(bb1->entity)->pos;
    auto bb2pos = m_entityManager.GetComponent<CTransform>(bb2->entity)->pos;

    float dx = abs(bb1pos.x - bb2pos.x);
    float dy = abs(bb1pos.y - bb2pos.y);
    if (  bb1->halfWidth  + bb2->halfWidth  - dx > 0   &&
          bb1->halfHeight + bb2->halfHeight - dy > 0     )
        return true;
    return false;
}
