#include "GeoWarsGame.h"

GeoWarsGame::GeoWarsGame()
    :m_window(sf::VideoMode(800, 600), "GeoWars")
{

}

void GeoWarsGame::Run()
{
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        m_window.clear(sf::Color::Black);
        m_window.display();
    }
}
