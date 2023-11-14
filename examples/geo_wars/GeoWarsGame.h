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
    bool m_paused = false;
    sf::Text m_text;
    int m_score = 0;

    void SUserInput();
    std::shared_ptr<Entity> SpawnPlayer();
    void SpawnEnemy();
    void SpawnSmallEnemies(const std::shared_ptr<Entity>& e);
    void SpawnBullet(int x, int y);
    void SEnemySpawner();
    void SMovement() const;
    void SRotate() const;
    void SCollision();
    void SRender();
    void SUpdateLifeSpan();
    void Restart();

    void SUpdateTransparencyBasedOnLifeSpan();

    void SDestroyEntitiesThatReachedEndOfTheirLifeSpan();

    int GetRandomNumberInRange(const int lower_bound, const int upper_bound);

};
