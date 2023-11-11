#pragma once
#include <SFML/Graphics.hpp>
#include "../../engine/EntityManager.h"

class GeoWarsGame
{
public:
    GeoWarsGame();
    void Run();

private:
    EntityManager m_entityManager;
    sf::RenderWindow m_window;
    std::shared_ptr<Entity> m_player;
    int m_currentFrame = 0;
    int m_lastEnemySpawnedFrame = 0;

    void SUserInput();
    std::shared_ptr<Entity> SpawnPlayer();
    void SpawnEnemy();
    void SpawnBullet(int x, int y);
    void SEnemySpawner();
    void SMovement() const;
    void SRender();
    int GetRandomNumberInRange(const int lower_bound, const int upper_bound);
};
