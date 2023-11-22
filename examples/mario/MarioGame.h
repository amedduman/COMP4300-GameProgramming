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

    void SpawnPlayer();
    void SpawnEnemies();
    void AddDebugShape(const std::shared_ptr<CShape>&);
    bool PointVsRect(Vec2 point, const std::shared_ptr<CBoxCollider>& rect);
    bool RectVsRect(const std::shared_ptr<CBoxCollider>& rect1, const std::shared_ptr<CBoxCollider>& rect2);

    void SInput();
    void SCalculateVelocity();
    void SMovement();
    void SDetectCollision();
    void SRender();
};
