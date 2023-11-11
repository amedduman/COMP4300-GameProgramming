#include "GeoWarsGame.h"
#include <random>

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
        SEnemySpawner();
        SUserInput();
        SMovement();
        SRender();

        m_currentFrame++;
    }
}

std::shared_ptr<Entity> GeoWarsGame::SpawnPlayer()
{
    auto player = m_entityManager.AddEntity("player");
    player->cTransform = std::make_shared<CTransform>(CTransform(Vec2(100,100), Vec2(1,1), 3));
    player->cShape = std::make_shared<CShape>(CShape(32,12,sf::Color::Transparent,sf::Color::White,3));
    player->cInput = std::make_shared<CInput>();
    return player;
}

void GeoWarsGame::SpawnEnemy()
{
    const auto enemy = m_entityManager.AddEntity("enemy");
    constexpr float radius = 32;
    const int points = GetRandomNumberInRange(3,8);
    const Vec2 pos(GetRandomNumberInRange(radius, m_window.getSize().x - radius),
        GetRandomNumberInRange(radius, m_window.getSize().y - radius));
    const Vec2 vel(GetRandomNumberInRange(-5,5),
        GetRandomNumberInRange(-5,5));
    enemy->cTransform = std::make_shared<CTransform>(CTransform(pos, vel, 0));
    enemy->cShape = std::make_shared<CShape>(CShape(radius,points,sf::Color::Transparent,sf::Color::Red,3));
}

void GeoWarsGame::SUserInput()
{
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
                    break;
                case sf::Keyboard::D:
                    m_player->cInput->right = true;
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
                default:
                    break;
            }
        }
        if(event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::A:
                    m_player->cInput->left = false;
                    break;
                case sf::Keyboard::D:
                    m_player->cInput->right = false;
                    break;
                case sf::Keyboard::W:
                    m_player->cInput->up = false;
                    break;
                case sf::Keyboard::S:
                    m_player->cInput->down = false;
                    break;
                default:
                    break;
            }
        }
    }
}

void GeoWarsGame::SEnemySpawner()
{
    if (m_currentFrame - m_lastEnemySpawnedFrame > 60 * 3)
    {
        SpawnEnemy();
        m_lastEnemySpawnedFrame = m_currentFrame;
    }
}

void GeoWarsGame::SMovement() const
{
    m_player->cTransform->velocity = Vec2(0,0);
    constexpr float speed = 8;
    if (m_player->cInput->right)
        m_player->cTransform->velocity.x = 1 * speed;
    else if (m_player->cInput->left)
        m_player->cTransform->velocity.x = -1 * speed;
    if (m_player->cInput->up)
        m_player->cTransform->velocity.y = -1 * speed;
    else if (m_player->cInput->down)
        m_player->cTransform->velocity.y = 1 * speed;

    for (auto& e : m_entityManager.GetEntities())
    {
        e->cTransform->pos.x += e->cTransform->velocity.x;
        e->cTransform->pos.y += e->cTransform->velocity.y;

        e->cTransform->pos.x = std::clamp(static_cast<int>(e->cTransform->pos.x), static_cast<int>(e->cShape->circle.getRadius()), static_cast<int>(m_window.getSize().x - e->cShape->circle.getRadius()));
        e->cTransform->pos.y = std::clamp(static_cast<int>(e->cTransform->pos.y), static_cast<int>(e->cShape->circle.getRadius()), static_cast<int>(m_window.getSize().y - e->cShape->circle.getRadius()));

        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

        if(e->cTransform->pos.x >= m_window.getSize().x - e->cShape->circle.getRadius() || e->cTransform->pos.x <= e->cShape->circle.getRadius())
            e->cTransform->velocity.x *= -1;
        if(e->cTransform->pos.y >= m_window.getSize().y - e->cShape->circle.getRadius() || e->cTransform->pos.y <= e->cShape->circle.getRadius())
            e->cTransform->velocity.y *= -1;
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

int GeoWarsGame::GetRandomNumberInRange(const int lower_bound, const int upper_bound)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distribution(lower_bound, upper_bound);

    const int random_number = distribution(gen);
    return random_number;
}

