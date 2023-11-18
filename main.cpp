#include "engine/ecs/Components.h"
#include "engine/ecs/EntityManager.h"

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>

class Animal {
public:
    virtual ~Animal() {}  // Ensure a virtual destructor in the base class for proper cleanup
};

class Dog : public Animal
{
public:
    std::string dogName;
    explicit Dog(const std::string& p_dogName)
    {
        dogName = p_dogName;
    }
    // Dog-specific members
};

class Cat : public Animal
{
public:
    std::string catName;
    explicit Cat(const std::string& p_catName)
    {
        catName = p_catName;
    }
};

class AnimalManager {
public:
    std::map<std::string, std::vector<std::shared_ptr<Animal>>> myAnimalMap;

    void AddAnimal(const std::shared_ptr<Animal>& a, const std::string& category)
    {
        myAnimalMap[category].push_back(a);
    }

    template<typename T>
    std::vector<std::shared_ptr<T>> GetAnimals()
    {
        std::vector<std::shared_ptr<T>> result;

        for (const auto& category : myAnimalMap)
        {
            for (const auto& animalPtr : category.second)
            {
                if (std::dynamic_pointer_cast<T>(animalPtr))
                {
                    result.push_back(std::dynamic_pointer_cast<T>(animalPtr));
                }
            }
        }

        return result;
    }
};

int main() {
    // AnimalManager animalManager;
    //
    // // Adding animals
    // animalManager.AddAnimal(std::make_shared<Dog>("first dog"), "Dogs");
    // animalManager.AddAnimal(std::make_shared<Cat>("first cat"), "Cats");
    // animalManager.AddAnimal(std::make_shared<Dog>("second dog"), "Dogs");
    // animalManager.AddAnimal(std::make_shared<Cat>("second cat"), "Cats");
    //
    // // Retrieving animals
    // auto dogs = animalManager.GetAnimals<Dog>();
    // auto cats = animalManager.GetAnimals<Cat>();
    //
    // // Use the retrieved vectors as needed
    // for (const auto& dog : dogs)
    // {
    //     std::cout << dog->dogName << std::endl;
    //     // Do something with dog
    // }
    //
    // for (const auto& cat : cats)
    // {
    //     std::cout << cat->catName << std::endl;
    //     // Do something with cat
    // }
    //
    // return 0;

    EntityManager emg;

    auto player = emg.AddEntity("player");
    emg.AddComponent<CTransform>(std::make_shared<CTransform>(player, Vec2(100, 100), Vec2(0,0)));
    emg.AddComponent<CShape>(std::make_shared<CShape>(player, 32, 32, sf::Color::Green, sf::Color::Red, 4));

    for (const auto& e : emg.GetComponents<CTransform>())
    {
        std::cout << e->pos.x << ", " << e->pos.y << std::endl;
    }
}
