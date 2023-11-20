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
    std::vector<sf::RectangleShape> m_debugShapes;

    void SpawnPlayer();
    void SpawnEnemies();
    void AddDebugShape(sf::Vector2f pos, sf::Vector2f size, sf::Color fCol = sf::Color::Transparent, sf::Color oCol = sf::Color::White, float thickness = 1);

    void SInput();
    void SCalculateVelocity();
    void SMovement();
    void SDetectCollision();
    void SRender();
};
