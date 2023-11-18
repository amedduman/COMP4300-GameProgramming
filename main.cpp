#include "engine/ecs/Components.h"
#include "engine/ecs/EntityManager.h"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    EntityManager emg;

    auto player = emg.AddEntity("player");
    emg.AddComponent(std::make_shared<CTransform>(player, Vec2(100, 100), Vec2(0,0)));
    emg.AddComponent(std::make_shared<CShape>(player, 32, 32, sf::Color::Green, sf::Color::Red, 4));

    for (const auto& e : emg.GetComponents<CTransform>())
    {
        std::cout << e->pos.x << ", " << e->pos.y << std::endl;
    }
}
