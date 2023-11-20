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

    void SpawnPlayer();
    void SpawnEnemies();

    void SInput();
    void SCalculateVelocity();
    void SMovement();
    void SRender();
};
