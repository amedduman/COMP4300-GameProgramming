// #include "GeoWarsGame.h"
// #include <iostream>
// #include <random>
// #include <cmath>

// GeoWarsGame::GeoWarsGame()
//     : m_entityManager(EntityManager()),
//       m_window(sf::VideoMode(800, 600), "GeoWars")
// {
// }

// void GeoWarsGame::Run()
// {
//     m_window.setFramerateLimit(60);
//     m_player = SpawnPlayer();

//     sf::Font font;
//     if (!font.loadFromFile("../fonts/CotaneBeach.otf"))
//     {
//         std::cout << "couldn't load the font" << std::endl;
//         exit(-1);
//     }

//     m_text = sf::Text("000", font, 18);

//     while (m_window.isOpen())
//     {
//         m_entityManager.Update();
//         SUserInput();

//         if (!m_paused)
//         {
//             SEnemySpawner();
//             SMovement();
//             SRotate();
//             SCollision();
//             SUpdateLifeSpan();
//             SUpdateTransparencyBasedOnLifeSpan();
//             SDestroyEntitiesThatReachedEndOfTheirLifeSpan();
//         }

//         SRender();
//         m_currentFrame++;
//     }
// }

// std::shared_ptr<Entity> GeoWarsGame::SpawnPlayer()
// {
//     auto player = m_entityManager.AddEntity("player");
//     player->cTransform = std::make_shared<CTransform>(CTransform(Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2), Vec2(1, 1), 3));
//     player->cShape = std::make_shared<CShape>(CShape(32, 12, sf::Color::Transparent, sf::Color::White, 3));
//     player->cInput = std::make_shared<CInput>();
//     player->cCollision = std::make_shared<CCollision>(32);
//     return player;
// }

// void GeoWarsGame::SpawnBullet(const int x, const int y)
// {
//     const auto bullet = m_entityManager.AddEntity("bullet");
//     constexpr float speed = 20;
//     Vec2 dir(0, 0);
//     dir.x = x - m_player->cTransform->pos.x;
//     dir.y = y - m_player->cTransform->pos.y;
//     const float len = sqrt(dir.x * dir.x + dir.y * dir.y);
//     dir.x = dir.x / len * speed;
//     dir.y = dir.y / len * speed;
//     bullet->cTransform = std::make_shared<CTransform>(CTransform(m_player->cTransform->pos, dir, 0));
//     bullet->cShape = std::make_shared<CShape>(CShape(8, 12, sf::Color::White, sf::Color::White, 3));
//     bullet->cCollision = std::make_shared<CCollision>(CCollision(8));
//     bullet->cLifeSpan = std::make_shared<CLifeSpan>(CLifeSpan(25));
// }

// void GeoWarsGame::SpawnEnemy()
// {
//     const auto enemy = m_entityManager.AddEntity("enemy");
//     constexpr float radius = 32;
//     const int points = GetRandomNumberInRange(3, 8);
//     const Vec2 pos(GetRandomNumberInRange(radius, m_window.getSize().x - radius),
//                    GetRandomNumberInRange(radius, m_window.getSize().y - radius));
//     const Vec2 vel(GetRandomNumberInRange(-5, 5),
//                    GetRandomNumberInRange(-5, 5));
//     enemy->cTransform = std::make_shared<CTransform>(CTransform(pos, vel, 0));
//     enemy->cShape = std::make_shared<CShape>(CShape(radius, points, sf::Color::Transparent, sf::Color::Red, 3));
//     enemy->cCollision = std::make_shared<CCollision>(CCollision(radius));
// }

// void GeoWarsGame::Restart()
// {
//     m_score = 0;

//     m_player->cTransform->pos = Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2);
//     m_player->cTransform->velocity = Vec2(0, 0);

//     for (auto &enemy : m_entityManager.GetEntities("enemy"))
//     {
//         enemy->Destroy();
//     }

//     for (auto &bullet : m_entityManager.GetEntities("bullet"))
//     {
//         bullet->Destroy();
//     }

//     for (auto &bullet : m_entityManager.GetEntities("small enemy"))
//     {
//         bullet->Destroy();
//     }

//     m_player->cInput->down = false;
//     m_player->cInput->up = false;
//     m_player->cInput->right = false;
//     m_player->cInput->left = false;
// }

// void GeoWarsGame::SUserInput()
// {
//     sf::Event event;
//     while (m_window.pollEvent(event))
//     {
//         if (event.type == sf::Event::Closed)
//             m_window.close();

//         if (event.type == sf::Event::MouseButtonPressed)
//         {
//             if (event.mouseButton.button == sf::Mouse::Left)
//             {
//                 SpawnBullet(event.mouseButton.x, event.mouseButton.y);
//             }
//         }

//         if (event.type == sf::Event::KeyPressed)
//         {
//             switch (event.key.code)
//             {
//             case sf::Keyboard::A:
//                 m_player->cInput->left = true;
//                 break;
//             case sf::Keyboard::D:
//                 m_player->cInput->right = true;
//                 break;
//             case sf::Keyboard::W:
//                 m_player->cInput->up = true;
//                 break;
//             case sf::Keyboard::S:
//                 m_player->cInput->down = true;
//                 break;
//             case sf::Keyboard::Escape:
//                 m_window.close();
//                 break;
//             case sf::Keyboard::P:
//                 m_paused = !m_paused;
//                 break;
//             default:
//                 break;
//             }
//         }
//         if (event.type == sf::Event::KeyReleased)
//         {
//             switch (event.key.code)
//             {
//             case sf::Keyboard::A:
//                 m_player->cInput->left = false;
//                 break;
//             case sf::Keyboard::D:
//                 m_player->cInput->right = false;
//                 break;
//             case sf::Keyboard::W:
//                 m_player->cInput->up = false;
//                 break;
//             case sf::Keyboard::S:
//                 m_player->cInput->down = false;
//                 break;
//             default:
//                 break;
//             }
//         }
//     }
// }

// void GeoWarsGame::SEnemySpawner()
// {
//     if (m_currentFrame - m_lastEnemySpawnedFrame > 60 * 3)
//     {
//         SpawnEnemy();
//         m_lastEnemySpawnedFrame = m_currentFrame;
//     }
// }

// void GeoWarsGame::SpawnSmallEnemies(const std::shared_ptr<Entity> &e)
// {
//     const float angle = static_cast<float>(360) / static_cast<float>(e->cShape->circle.getPointCount());
//     constexpr float PI = 3.14159265f;

//     for (int i = 0; i < e->cShape->circle.getPointCount(); i++)
//     {
//         const auto enemy = m_entityManager.AddEntity("small enemy");
//         constexpr float radius = 12;
//         const int points = static_cast<int>(e->cShape->circle.getPointCount());
//         const Vec2 pos = e->cTransform->pos;
//         Vec2 vel = Vec2(0, -1);
//         Vec2 tmp = Vec2(0, 0);

//         constexpr float speed = 5;
//         const float radians = angle * i * (PI / 180.0f);
//         tmp.x = cos(radians) * vel.x - sin(radians) * vel.y;
//         tmp.y = sin(radians) * vel.x + cos(radians) * vel.y;
//         vel.x = tmp.x * speed;
//         vel.y = tmp.y * speed;
//         // std::cout << "degree is " << angle * i  << "/ " << "radians is " << radians << ": " << cos(radians) + 1 << std::endl;
//         // std::cout << vel.x << ", " << vel.y << std::endl;

//         enemy->cTransform = std::make_shared<CTransform>(CTransform(pos, vel, 0));
//         enemy->cShape = std::make_shared<CShape>(CShape(radius, points, sf::Color::Transparent, sf::Color::Red, 3));
//         enemy->cCollision = std::make_shared<CCollision>(CCollision(radius));
//         enemy->cLifeSpan = std::make_shared<CLifeSpan>(CLifeSpan(25));
//     }
// }

// void GeoWarsGame::SMovement() const
// {
//     m_player->cTransform->velocity = Vec2(0, 0);
//     constexpr float speed = 8;
//     if (m_player->cInput->right)
//         m_player->cTransform->velocity.x = 1 * speed;
//     else if (m_player->cInput->left)
//         m_player->cTransform->velocity.x = -1 * speed;
//     if (m_player->cInput->up)
//         m_player->cTransform->velocity.y = -1 * speed;
//     else if (m_player->cInput->down)
//         m_player->cTransform->velocity.y = 1 * speed;

//     for (auto &e : m_entityManager.GetEntities())
//     {
//         e->cTransform->pos.x += e->cTransform->velocity.x;
//         e->cTransform->pos.y += e->cTransform->velocity.y;

//         e->cTransform->pos.x = std::clamp(static_cast<int>(e->cTransform->pos.x), static_cast<int>(e->cShape->circle.getRadius()), static_cast<int>(m_window.getSize().x - e->cShape->circle.getRadius()));
//         e->cTransform->pos.y = std::clamp(static_cast<int>(e->cTransform->pos.y), static_cast<int>(e->cShape->circle.getRadius()), static_cast<int>(m_window.getSize().y - e->cShape->circle.getRadius()));

//         e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

//         if (e->cTransform->pos.x >= m_window.getSize().x - e->cShape->circle.getRadius() || e->cTransform->pos.x <= e->cShape->circle.getRadius())
//             e->cTransform->velocity.x *= -1;
//         if (e->cTransform->pos.y >= m_window.getSize().y - e->cShape->circle.getRadius() || e->cTransform->pos.y <= e->cShape->circle.getRadius())
//             e->cTransform->velocity.y *= -1;
//     }
// }

// void GeoWarsGame::SRotate() const
// {
//     for (auto &e : m_entityManager.GetEntities())
//     {
//         e->cShape->circle.rotate(6);
//     }
// }

// void GeoWarsGame::SCollision()
// {
//     for (auto &e : m_entityManager.GetEntities("enemy"))
//     {
//         const Vec2 diffP(e->cTransform->pos.x - m_player->cTransform->pos.x, e->cTransform->pos.y - m_player->cTransform->pos.y);
//         const float lenP = sqrt(diffP.x * diffP.x + diffP.y * diffP.y);
//         if (lenP <= e->cCollision->radius + m_player->cCollision->radius)
//         {
//             Restart();
//             break;
//         }

//         for (auto &b : m_entityManager.GetEntities("bullet"))
//         {
//             const Vec2 diff(e->cTransform->pos.x - b->cTransform->pos.x, e->cTransform->pos.y - b->cTransform->pos.y);
//             const float len = sqrt(diff.x * diff.x + diff.y * diff.y);
//             if (len <= e->cCollision->radius + b->cCollision->radius)
//             {
//                 m_score += e->cShape->circle.getPointCount() * 10;
//                 SpawnSmallEnemies(e);
//                 b->Destroy();
//                 e->Destroy();
//             }
//         }
//     }

//     for (auto &e : m_entityManager.GetEntities("small enemy"))
//     {
//         const Vec2 diffP(e->cTransform->pos.x - m_player->cTransform->pos.x, e->cTransform->pos.y - m_player->cTransform->pos.y);
//         const float lenP = sqrt(diffP.x * diffP.x + diffP.y * diffP.y);
//         if (lenP <= e->cCollision->radius + m_player->cCollision->radius)
//         {
//             Restart();
//             break;
//         }

//         for (auto &b : m_entityManager.GetEntities("bullet"))
//         {
//             const Vec2 diff(e->cTransform->pos.x - b->cTransform->pos.x, e->cTransform->pos.y - b->cTransform->pos.y);
//             const float len = sqrt(diff.x * diff.x + diff.y * diff.y);
//             if (len <= e->cCollision->radius + b->cCollision->radius)
//             {
//                 if (b->IsAlive() == false) // since we destroy the bullet that killed the original enemy we don't want our small enemies also die with same bullet.
//                 {
//                     m_score += e->cShape->circle.getPointCount() * 20;
//                     e->Destroy();
//                 }
//             }
//         }
//     }
// }

// void GeoWarsGame::SRender()
// {
//     m_window.clear(sf::Color::Black);
//     for (auto &e : m_entityManager.GetEntities())
//     {
//         if (e->cShape)
//         {
//             m_window.draw(e->cShape->circle);
//         }
//     }
//     m_text.setString(std::to_string(m_score));
//     m_window.draw(m_text);
//     m_window.display();
// }

// void GeoWarsGame::SUpdateLifeSpan()
// {
//     for (auto &e : m_entityManager.GetEntities())
//     {
//         if (e->cLifeSpan)
//         {
//             e->cLifeSpan->remaining -= 1;

//             if (e->cLifeSpan->remaining <= 0)
//             {
//                 e->cLifeSpan->remaining = 0;
//             }
//         }
//     }
// }

// void GeoWarsGame::SUpdateTransparencyBasedOnLifeSpan()
// {
//     for (auto &e : m_entityManager.GetEntities())
//     {
//         if (e->cLifeSpan)
//         {
//             auto fCol = e->cShape->circle.getFillColor();
//             auto oCol = e->cShape->circle.getOutlineColor();

//             const float ratio = static_cast<float>(e->cLifeSpan->remaining) / static_cast<float>(e->cLifeSpan->total);
//             const float new_a = 255 * ratio;

//             fCol.a = static_cast<int>(new_a);
//             oCol.a = static_cast<int>(new_a);
//             e->cShape->circle.setFillColor(fCol);
//             e->cShape->circle.setOutlineColor(oCol);
//         }
//     }
// }

// void GeoWarsGame::SDestroyEntitiesThatReachedEndOfTheirLifeSpan()
// {
//     for (auto &e : m_entityManager.GetEntities())
//     {
//         if (e->cLifeSpan)
//         {
//             if (e->cLifeSpan->remaining <= 0)
//             {
//                 e->Destroy();
//             }
//         }
//     }
// }

// int GeoWarsGame::GetRandomNumberInRange(const int lower_bound, const int upper_bound)
// {
//     std::random_device rd;
//     std::mt19937 gen(rd());

//     std::uniform_int_distribution<int> distribution(lower_bound, upper_bound);

//     const int random_number = distribution(gen);
//     return random_number;
// }
