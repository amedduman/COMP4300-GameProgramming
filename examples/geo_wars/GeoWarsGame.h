#pragma once
#include <SFML/Graphics.hpp>

class GeoWarsGame
{
public:
    GeoWarsGame();
    void Run();
private:
    sf::RenderWindow m_window;
};
