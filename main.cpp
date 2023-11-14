#include <iomanip>
#include <iostream>
#include <math.h>

#include "examples/geo_wars/GeoWarsGame.h"

double cos_d (double degrees)
{
    return (cos(degrees * 3.1416/180));
}

int main()
{
    // std::cout << "Cosine of 90 degrees is: " << cos_d(90) + 1 << std::endl;


    // std::cout << "Cosine of " << 90 << " degrees is: " << cos_d(90) << std::endl;
    //
    // return 0;

    // constexpr float PI = 3.14159265f;
    //
    // double angle = 90;
    // double radians = angle * (PI / 180.0);
    // std::cout << cos(radians) << std::endl;
    GeoWarsGame game;
    game.Run();
}