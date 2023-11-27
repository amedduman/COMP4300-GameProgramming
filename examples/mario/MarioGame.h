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
    std::vector<std::shared_ptr<CShape>> m_debugShapes;
    int m_frameCount = 0;

    void SpawnPlayer();
    void SpawnEnemies();
    void AddDebugShape(const std::shared_ptr<CShape>&);
    bool PointVsRect(Vec2 point, const std::shared_ptr<CBoundingBox>& rect);
    Vec2 RectVsRect(const std::shared_ptr<CBoundingBox>& rect1, const std::shared_ptr<CBoundingBox>& rect2, bool doForPreviousPos = false);
    void Reselotion();

    void SInput();
    void SCalculateVelocity();
    void SMovement();
    void SDetectCollision();
    void SRender();
};
