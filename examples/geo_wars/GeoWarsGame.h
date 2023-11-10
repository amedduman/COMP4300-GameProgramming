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

    void SpawnPlayer();
    void SMovement();
};
