#pragma once
#include <SFML/Graphics.hpp>
#include "../../engine/ecs/EntityManager.h"

class MarioGame
{
public:
    MarioGame();
    void Run();
private:
    sf::RenderWindow m_window;
    EntityManager m_entityManager;
    int m_frameCount = 0;

    void SpawnPlayer();
    void SpawnTiles();
    void SpawnEnemies();
    Vec2 RectVsRect(const std::shared_ptr<CBoundingBox>& rect1, const std::shared_ptr<CBoundingBox>& rect2, bool doForPreviousPos = false);
    void Reselotion();
    void Reselotion(std::shared_ptr<CBoundingBox> bb);
    void SyncShapeAndTransform();

    void SInput();
    void SCalculateVelocity();
    void SMovement();
    void SDetectCollision();
    void SRender();
};
