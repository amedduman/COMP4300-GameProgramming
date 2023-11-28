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
        m_frameCount ++;

        SInput();
        SCalculateVelocity();
        SMovement();
        SDetectCollision();
        // Reselotion();
        SyncShapeAndTransform();
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

    auto enemy2 = m_entityManager.AddEntity("enemy");
    constexpr float sizeX2 = 100;
    constexpr float sizeY2 = 100;
    m_entityManager.AddComponent(std::make_shared<CTransform>(enemy2, Vec2(m_window.getSize().x / 4 + 100, m_window.getSize().y / 4)));
    m_entityManager.AddComponent(std::make_shared<CShape>(enemy2, sf::Vector2f(sizeX2, sizeY2), sf::Color::Transparent, sf::Color::Yellow, 1));
    m_entityManager.AddComponent(std::make_shared<CVelocity>(enemy2));
    m_entityManager.AddComponent(std::make_shared<CBoundingBox>(enemy2, Vec2(sizeY2, sizeY2)));
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
        auto vel = m_entityManager.GetComponent<CVelocity>(e);
        if (tr && vel)
        {
            tr->SetPos(tr->GetPos().x + vel->velocity.x, tr->GetPos().y + vel->velocity.y);
        }
    }
}
void MarioGame::SDetectCollision()
{
    auto player = m_entityManager.GetComponent<CBoundingBox>("player");
    for (auto& e : m_entityManager.GetEntities())
    {
        if (e->Tag() == "player") continue; // don't collide with self
        auto bb = m_entityManager.GetComponent<CBoundingBox>(e);
        if (bb && player)
        {
            auto overlapArea = RectVsRect(player, bb);
            if (overlapArea.x > 0 && overlapArea.y > 0)
            {
                Reselotion(bb);
            }
        }
    }
    //
    // auto rectE = m_entityManager.GetComponent<CBoundingBox>("enemy");
    // auto rectP = m_entityManager.GetComponent<CBoundingBox>("player");
    //
    // auto overlapArea = RectVsRect(rectE, rectP);
    // if (overlapArea.x > 0 && overlapArea.y > 0)
    //     m_entityManager.GetComponent<CShape>("enemy")->rect.setOutlineColor(sf::Color::Green);
    // else
    //     m_entityManager.GetComponent<CShape>("enemy")->rect.setOutlineColor(sf::Color::Yellow);
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

Vec2 MarioGame::RectVsRect(const std::shared_ptr<CBoundingBox>& bb1, const std::shared_ptr<CBoundingBox>& bb2, bool doForPreviousPos)
{
    Vec2 bb1pos;
    Vec2 bb2pos;
    if (doForPreviousPos == false)
    {
        bb1pos = m_entityManager.GetComponent<CTransform>(bb1->entity)->GetPos();
        bb2pos = m_entityManager.GetComponent<CTransform>(bb2->entity)->GetPos();
    }
    else
    {
        bb1pos = m_entityManager.GetComponent<CTransform>(bb1->entity)->GetPreviousPos();
        bb2pos = m_entityManager.GetComponent<CTransform>(bb2->entity)->GetPreviousPos();
    }

    float dx = abs(bb1pos.x - bb2pos.x);
    float dy = abs(bb1pos.y - bb2pos.y);
    Vec2 overlapArea = Vec2(bb1->halfWidth + bb2->halfWidth - dx, bb1->halfHeight + bb2->halfHeight - dy);
    return overlapArea;
}
void MarioGame::Reselotion()
{
    auto rectE = m_entityManager.GetComponent<CBoundingBox>("enemy");
    auto rectP = m_entityManager.GetComponent<CBoundingBox>("player");

    auto trE = m_entityManager.GetComponent<CTransform>("enemy");
    auto trP = m_entityManager.GetComponent<CTransform>("player");

    auto overlapArea = RectVsRect(rectE, rectP);
    if (overlapArea.x > 0 && overlapArea.y > 0)
    {
        auto previousOverlapArea = RectVsRect(rectE, rectP, true);
        if(previousOverlapArea.y > 0)
        {
            std::cout << "move in x direction to resolve the collision" << std::endl;
            if (trP->GetPos().x > trP->GetPreviousPos().x)
            {
                trP->SetPos(trP->GetPos().x - overlapArea.x, trP->GetPos().y);
            }
            else
            {
                trP->SetPos(trP->GetPos().x + overlapArea.x, trP->GetPos().y);
            }
        }
        if(previousOverlapArea.x > 0)
        {
            std::cout << "move in y direction to resolve the collision" << std::endl;
            if (trP->GetPos().y > trP->GetPreviousPos().y)
            {
                trP->SetPos(trP->GetPos().x, trP->GetPos().y  - overlapArea.y);
            }
            else
            {
                trP->SetPos(trP->GetPos().x, trP->GetPos().y  + overlapArea.y);
            }
        }
    }
}

void MarioGame::Reselotion(const std::shared_ptr<CBoundingBox> bb)
{
    // auto rectE = m_entityManager.GetComponent<CBoundingBox>("enemy");
    auto rectP = m_entityManager.GetComponent<CBoundingBox>("player");
    auto trP = m_entityManager.GetComponent<CTransform>("player");
    // auto tr_bb = m_entityManager.GetComponent<CTransform>(bb->entity);

    auto overlapArea = RectVsRect(bb, rectP);
    if (overlapArea.x > 0 && overlapArea.y > 0)
    {
        auto previousOverlapArea = RectVsRect(bb, rectP, true);
        if(previousOverlapArea.y > 0)
        {
            std::cout << "move in x direction to resolve the collision" << std::endl;
            if (trP->GetPos().x > trP->GetPreviousPos().x)
            {
                trP->SetPos(trP->GetPos().x - overlapArea.x, trP->GetPos().y);
            }
            else
            {
                trP->SetPos(trP->GetPos().x + overlapArea.x, trP->GetPos().y);
            }
        }
        if(previousOverlapArea.x > 0)
        {
            std::cout << "move in y direction to resolve the collision" << std::endl;
            if (trP->GetPos().y > trP->GetPreviousPos().y)
            {
                trP->SetPos(trP->GetPos().x, trP->GetPos().y  - overlapArea.y);
            }
            else
            {
                trP->SetPos(trP->GetPos().x, trP->GetPos().y  + overlapArea.y);
            }
        }
    }
}

void MarioGame::SyncShapeAndTransform()
{
    for (auto& e : m_entityManager.GetEntities())
    {
        auto tr = m_entityManager.GetComponent<CTransform>(e);
        auto shape = m_entityManager.GetComponent<CShape>(e);
        if (tr && shape)
        {
            shape->rect.setPosition(tr->GetPos().x, tr->GetPos().y);
        }
    }
}
