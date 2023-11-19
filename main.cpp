#include "engine/ecs/Components.h"
#include "engine/ecs/EntityManager.h"
#include <iostream>
#include <string>
void SMovement(EntityManager& emg);
int main()
{
    EntityManager emg;

    auto player = emg.AddEntity("player");
    emg.AddComponent(std::make_shared<CTransform>(player, Vec2(100, 100), Vec2(0,0)));
    emg.AddComponent(std::make_shared<CShape>(player, 32, 32, sf::Color::Green, sf::Color::Red, 4));

    auto enemy = emg.AddEntity("enemy");
    emg.AddComponent(std::make_shared<CTransform>(enemy, Vec2(200, 200), Vec2(0,0)));
    emg.AddComponent(std::make_shared<CShape>(enemy, 32, 32, sf::Color::White, sf::Color::Red, 4));

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        emg.Update();

        for (auto& e : emg.GetEntities())
        {
            auto tr = emg.GetComponent<CTransform>(e);
            auto shape = emg.GetComponent<CShape>(e);
            if (tr && shape)
            {
                shape->circle.setPosition(tr->pos.x, tr->pos.y);
            }
        }

        window.clear(sf::Color::Black);
        for (auto& e : emg.GetComponents<CShape>())
        {
            window.draw(e->circle);
        }
        window.display();
    }

}

void SMovement(EntityManager& emg)
{
    for (auto& e : emg.GetEntities())
    {
        printf("so");
        auto& tr = emg.GetComponent<CTransform>(e);
        auto& shape = emg.GetComponent<CShape>(e);
        if (tr && shape)
        {
            printf("so");
            shape->circle.setPosition(tr->pos.x, tr->pos.y);
        }
    }
}

