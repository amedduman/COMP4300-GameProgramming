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
    SpawnTiles();

    while (m_window.isOpen())
    {
        m_entityManager.Update();
        m_frameCount ++;

        SInput();
        SCalculateVelocity();
        // SApplyGravity();
        SMovement();
        SDetectCollision();
        SAnimate();
        SyncShapeAndTransform();
        SyncSpriteAndTransform();
        MoveCamera();
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
    m_entityManager.AddComponent(std::make_shared<CGravity>(player, 9));

    if (!playerRunTex.loadFromFile("../examples/mario/run.png"))
    {
        std::cout << "error while loading image" << std::endl;
    }
    playerRunTex.setSmooth(true);
    if (!playerIdleTex.loadFromFile("../examples/mario/idle.png"))
    {
        std::cout << "error while loading image" << std::endl;
    }
    playerIdleTex.setSmooth(true);
    m_entityManager.AddComponent(std::make_shared<CSprite>(player, playerRunTex, sf::IntRect(0,0,48, 48), Vec2(28,28)));

    auto sprite = m_entityManager.GetComponent<CSprite>(player);

    std::vector<sf::IntRect> runFrames;
    runFrames.emplace_back(0,0,48,48);
    runFrames.emplace_back(48,0,48,48);
    runFrames.emplace_back(48*2,0,48,48);
    runFrames.emplace_back(48*3,0,48,48);
    runFrames.emplace_back(48*4,0,48,48);
    runFrames.emplace_back(48*5,0,48,48);
    runFrames.emplace_back(48*6,0,48,48);
    runFrames.emplace_back(48*7,0,48,48);

    std::vector<sf::IntRect> idleFrames;
    idleFrames.emplace_back(0,0,48,48);
    idleFrames.emplace_back(48,0,48,48);
    idleFrames.emplace_back(48*2,0,48,48);
    idleFrames.emplace_back(48*3,0,48,48);
    idleFrames.emplace_back(48*4,0,48,48);
    idleFrames.emplace_back(48*5,0,48,48);
    idleFrames.emplace_back(48*6,0,48,48);
    idleFrames.emplace_back(48*7,0,48,48);
    idleFrames.emplace_back(48*8,0,48,48);
    idleFrames.emplace_back(48*9,0,48,48);

    sprite->AddAnimation(playerRunTex, runFrames, "run", 5);
    sprite->AddAnimation(playerIdleTex, runFrames, "idle", 5);
}
void MarioGame::SpawnTiles()
{
    if (!tilemapTex.loadFromFile("../examples/mario/tilemap.png"))
    {
        std::cout << "error while loading image" << std::endl;
    }
    tilemapTex.setSmooth(true);


    constexpr float sizeX = 32;
    constexpr float sizeY = 32;

    // ground tiles and hazards
    for (int i = 0; i < 12; ++i)
    {
        auto tile = m_entityManager.AddEntity("tile");
        m_entityManager.AddComponent(std::make_shared<CTransform>(tile, Vec2(sizeX * i + sizeX/2, 500)));
        m_entityManager.AddComponent(std::make_shared<CShape>(tile, sf::Vector2f(sizeX, sizeY), sf::Color::Transparent));
        m_entityManager.AddComponent(std::make_shared<CBoundingBox>(tile, Vec2(sizeX, sizeY)));
        m_entityManager.AddComponent(std::make_shared<CSprite>(tile,tilemapTex, sf::IntRect(32,64,32,32), Vec2(16,16)));

        if(i == 8 || i == 9)
        {
            if (!hazardTex.loadFromFile("../examples/mario/hazard.png"))
            {
                std::cout << "error while loading image" << std::endl;
            }
            hazardTex.setSmooth(true);

            constexpr float enemySizeX = 32;
            constexpr float enemySizeY = 32;

            auto enemy = m_entityManager.AddEntity("enemy");
            m_entityManager.AddComponent(std::make_shared<CTransform>(enemy, Vec2(enemySizeX * i + enemySizeX/2, 500 - 32)));
            m_entityManager.AddComponent(std::make_shared<CShape>(enemy, sf::Vector2f(enemySizeX, enemySizeY), sf::Color::Transparent, sf::Color::Red, 1));
            m_entityManager.AddComponent(std::make_shared<CBoundingBox>(enemy, Vec2(enemySizeX, enemySizeY)));
            m_entityManager.AddComponent(std::make_shared<CSprite>(enemy, hazardTex, sf::IntRect(0,0, 32, 32), Vec2(16,16)));
        }
    }
    for (int i = 0; i < 18; ++i)
    {
        auto tile = m_entityManager.AddEntity("tile");
        m_entityManager.AddComponent(std::make_shared<CTransform>(tile, Vec2(sizeX * i + 550, 500)));
        m_entityManager.AddComponent(std::make_shared<CShape>(tile, sf::Vector2f(sizeX, sizeY), sf::Color::Transparent));
        m_entityManager.AddComponent(std::make_shared<CBoundingBox>(tile, Vec2(sizeX, sizeY)));
        m_entityManager.AddComponent(std::make_shared<CSprite>(tile,tilemapTex, sf::IntRect(32,64,32,32), Vec2(16,16)));
    }

    // hanging tiles
    for (int i = 0; i < 4; ++i)
    {
        if (i < 3)
        {
            auto tile = m_entityManager.AddEntity("tile");
            m_entityManager.AddComponent(std::make_shared<CTransform>(tile, Vec2(sizeX * i + 50, 400)));
            m_entityManager.AddComponent(std::make_shared<CShape>(tile, sf::Vector2f(sizeX, sizeY), sf::Color::Transparent));
            m_entityManager.AddComponent(std::make_shared<CBoundingBox>(tile, Vec2(sizeX, sizeY)));
            m_entityManager.AddComponent(std::make_shared<CSprite>(tile,tilemapTex, sf::IntRect(32,64,32,32), Vec2(16,16)));
        }
        else
        {
            if (!specialTileTex.loadFromFile("../examples/mario/specialTile.png"))
            {
                std::cout << "error while loading image" << std::endl;
            }
            specialTileTex.setSmooth(true);

            auto specialTile = m_entityManager.AddEntity("special tile");
            m_entityManager.AddComponent(std::make_shared<CTransform>(specialTile, Vec2(sizeX * i + 50, 400)));
            m_entityManager.AddComponent(std::make_shared<CShape>(specialTile, sf::Vector2f(sizeX, sizeY), sf::Color::Transparent, sf::Color::Yellow));
            m_entityManager.AddComponent(std::make_shared<CBoundingBox>(specialTile, Vec2(sizeX, sizeY)));
            m_entityManager.AddComponent(std::make_shared<CSprite>(specialTile, specialTileTex, sf::IntRect(0,0,32,32), Vec2(16,16)));
        }
    }
    for (int i = 0; i < 4; ++i)
    {
        if (i < 3)
        {
            auto tile = m_entityManager.AddEntity("tile");
            m_entityManager.AddComponent(std::make_shared<CTransform>(tile, Vec2(sizeX * i + 650, 400)));
            m_entityManager.AddComponent(std::make_shared<CShape>(tile, sf::Vector2f(sizeX, sizeY), sf::Color::Transparent));
            m_entityManager.AddComponent(std::make_shared<CBoundingBox>(tile, Vec2(sizeX, sizeY)));
            m_entityManager.AddComponent(std::make_shared<CSprite>(tile,tilemapTex, sf::IntRect(32,64,32,32), Vec2(16,16)));
        }
        else
        {
            if (!specialTileTex.loadFromFile("../examples/mario/specialTile.png"))
            {
                std::cout << "error while loading image" << std::endl;
            }
            specialTileTex.setSmooth(true);

            auto specialTile = m_entityManager.AddEntity("special tile");
            m_entityManager.AddComponent(std::make_shared<CTransform>(specialTile, Vec2(sizeX * i + 650, 400)));
            m_entityManager.AddComponent(std::make_shared<CShape>(specialTile, sf::Vector2f(sizeX, sizeY), sf::Color::Transparent, sf::Color::Yellow));
            m_entityManager.AddComponent(std::make_shared<CBoundingBox>(specialTile, Vec2(sizeX, sizeY)));
            m_entityManager.AddComponent(std::make_shared<CSprite>(specialTile, specialTileTex, sf::IntRect(0,0,32,32), Vec2(16,16)));
        }
    }
}
void MarioGame::SpawnBullet()
{
    if (!bulletTex.loadFromFile("../examples/mario/bullet.png"))
    {
        std::cout << "error while loading image" << std::endl;
    }
    bulletTex.setSmooth(true);

    constexpr float sizeX = 16;
    constexpr float sizeY = 16;

    auto playerPos = m_entityManager.GetComponent<CTransform>("player")->GetPos();

    auto bullet = m_entityManager.AddEntity("bullet");
    m_entityManager.AddComponent(std::make_shared<CTransform>(bullet, playerPos));
    m_entityManager.AddComponent(std::make_shared<CShape>(bullet, sf::Vector2f(sizeX, sizeY), sf::Color::Transparent, sf::Color::Green, 1));
    m_entityManager.AddComponent(std::make_shared<CSprite>(bullet, bulletTex, sf::IntRect(0,4,8,8), Vec2(4,4)));
    m_entityManager.AddComponent(std::make_shared<CBoundingBox>(bullet, Vec2(sizeY, sizeY)));
    m_entityManager.AddComponent(std::make_shared<CVelocity>(bullet, Vec2(10, 0)));
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
            if(event.key.code == sf::Keyboard::Space)
            {
                m_countOfJumpInputPressed++;
                if(m_countOfJumpInputPressed <= 1)
                {
                    input->jump = true;
                }
                else
                {
                    input->jump = false;
                }
            }
            if(event.key.code == sf::Keyboard::J) SpawnBullet();
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
            if(event.key.code == sf::Keyboard::Space)
            {
                input->jump = false;
                m_countOfJumpInputPressed = 0;
            }
        }
    }
}
void MarioGame::SCalculateVelocity()
{
    auto playerVel = m_entityManager.GetComponent<CVelocity>("player");
    auto playerInp = m_entityManager.GetComponent<CMarioInput>("player");
    playerVel->velocity = Vec2(0,0);
    constexpr float speed = 6;
    if (playerInp->moveUP) playerVel->velocity.y = -speed;
    if (playerInp->moveDown) playerVel->velocity.y = speed;
    if (playerInp->moveRight) playerVel->velocity.x = speed;
    if (playerInp->moveLeft) playerVel->velocity.x = -speed;
    if(playerInp->jump)
    {
        playerVel->velocity.y += -20;
    }
}
void MarioGame::SMovement()
{
    for(auto& e : m_entityManager.GetEntities())
    {
        auto tr = m_entityManager.GetComponent<CTransform>(e);
        auto vel = m_entityManager.GetComponent<CVelocity>(e);
        auto gravity = m_entityManager.GetComponent<CGravity>(e);
        if (tr && vel && gravity)
        {
            if(e->Tag() == "player")
            {
                if(tr->GetPos().x <= 0)
                {
                    tr->SetPos(1 , tr->GetPos().y + vel->velocity.y + gravity->GetGravity());
                }
                else if (tr->GetPos().x >= 1000)
                {
                    tr->SetPos(999 , tr->GetPos().y + vel->velocity.y + gravity->GetGravity());
                }
                else
                {
                    tr->SetPos(tr->GetPos().x + vel->velocity.x, tr->GetPos().y + vel->velocity.y + gravity->GetGravity());
                }
            }
            else
            {
                tr->SetPos(tr->GetPos().x + vel->velocity.x, tr->GetPos().y + vel->velocity.y + gravity->GetGravity());
            }
        }
        else if (tr && vel)
        {
            tr->SetPos(tr->GetPos().x + vel->velocity.x, tr->GetPos().y + vel->velocity.y);
        }
    }
}
void MarioGame::SApplyGravity()
{

    auto tr = m_entityManager.GetComponent<CTransform>("player");
    auto gravity = m_entityManager.GetComponent<CGravity>("player");
    if (tr && gravity)
    {

        tr->SetPos(tr->GetPos().x, tr->GetPos().y + 1);
    }

}

void MarioGame::SDetectCollision()
{
    auto player = m_entityManager.GetComponent<CBoundingBox>("player");
    for (auto& e : m_entityManager.GetEntities())
    {
        if (e->Tag() == "player" || e->Tag() == "bullet") continue; // don't collide with self and bullet
        auto bb = m_entityManager.GetComponent<CBoundingBox>(e);
        if (bb && player)
        {
            auto overlapArea = RectVsRect(player, bb);
            if (overlapArea.x > 0 && overlapArea.y > 0)
            {
                if (e->Tag() == "enemy")
                {
                    e->Destroy();
                    std::cout << "game over" << std::endl;
                }
                else if (e->Tag() == "special tile")
                {
                    auto tr = m_entityManager.GetComponent<CTransform>(player->entity);
                    if(tr->GetPos().y > tr->GetPreviousPos().y)
                    {
                        Reselotion(bb);
                    }
                    else
                    {
                        e->Destroy();
                    }
                }
                else
                {
                    Reselotion(bb);
                }
            }
        }
    }

    for (auto& b : m_entityManager.GetEntities("bullet"))
    {
        for (auto& e : m_entityManager.GetEntities("enemy"))
        {
            auto bulletBb = m_entityManager.GetComponent<CBoundingBox>(b);
            auto enemyBb = m_entityManager.GetComponent<CBoundingBox>(e);
            if ((bulletBb && enemyBb) == false ) std::cout << "can't get required components for detecting collision between bullet  and enemy" << std::endl;
            auto overlapArea = RectVsRect(bulletBb, enemyBb);
                if (overlapArea.x > 0 && overlapArea.y > 0)
                {
                    e->Destroy();
                    b->Destroy();
                    goto exitloop; // for this particular frame we want to exit form loop because the current
                }
        }

        for (auto& e : m_entityManager.GetEntities("tile"))
        {
            auto bulletBb = m_entityManager.GetComponent<CBoundingBox>(b);
            auto tileBb = m_entityManager.GetComponent<CBoundingBox>(e);
            if ((bulletBb && tileBb) == false ) std::cout << "can't get required components for detecting collision between bullet  and enemy" << std::endl;
            auto overlapArea = RectVsRect(bulletBb, tileBb);
            if (overlapArea.x > 0 && overlapArea.y > 0)
            {
                e->Destroy();
                b->Destroy();
                goto exitloop; // for this particular frame we want to exit form loop because the current
            }
        }
    }
    exitloop:
}

void MarioGame::SAnimate()
{
    auto playerSprite = m_entityManager.GetComponent<CSprite>("player");
    auto playerTr = m_entityManager.GetComponent<CTransform>("player");
    auto playerVel = m_entityManager.GetComponent<CVelocity>("player");

    if (playerVel->velocity.x > 0)
        playerSprite->GetSprite().setScale(1, 1);
    else if(playerVel->velocity.x < 0)
        playerSprite->GetSprite().setScale(-1, 1);

    if (playerVel->velocity.x != 0 || playerVel->velocity.y != 0)
        playerSprite->PLayAnimation("run");
    else
        playerSprite->PLayAnimation("idle");
}

void MarioGame::SRender()
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
void MarioGame::Reselotion(const std::shared_ptr<CBoundingBox> bb)
{
    auto rectP = m_entityManager.GetComponent<CBoundingBox>("player");
    auto trP = m_entityManager.GetComponent<CTransform>("player");

    auto overlapArea = RectVsRect(bb, rectP);
    if (overlapArea.x > 0 && overlapArea.y > 0)
    {
        auto previousOverlapArea = RectVsRect(bb, rectP, true);
        if(previousOverlapArea.y > 0)
        {
            // move in x direction to resolve the collision
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
            // move in y direction to resolve the collision
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

void MarioGame::SyncSpriteAndTransform()
{
    for (auto& e : m_entityManager.GetEntities())
    {
        auto tr = m_entityManager.GetComponent<CTransform>(e);
        auto sprite = m_entityManager.GetComponent<CSprite>(e);
        if (tr && sprite)
        {
            sprite->SetPos(tr->GetPos());
        }
    }
}
void MarioGame::MoveCamera()
{
    auto playerTr = m_entityManager.GetComponent<CTransform>("player");
    auto offsetX = playerTr->GetPos().x - 100;
    if (offsetX < 0) offsetX = 0;
    if(offsetX > 300) offsetX = 300;
    sf::View view(sf::FloatRect(0,0, 800, 600));
    view.move(offsetX,0);
    m_window.setView(view);
}
