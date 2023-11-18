#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Entity.h"
#include "Components.h"

class EntityManager
{
    typedef std::vector<std::shared_ptr<Entity>> EntityVec;
    typedef std::map <std::string, EntityVec> EntityMap;

public:
    EntityManager();
    void Update();
    std::shared_ptr<Entity> AddEntity(const std::string& tag = "default");
    const EntityVec& GetEntities() const;
    const EntityVec& GetEntities(const std::string& tag);
    template<typename T> void AddComponent(const std::shared_ptr<Component>& cmp);
    template<typename T> std::vector<std::shared_ptr<T>> GetComponents();
private:
    EntityVec m_entities;
    EntityVec m_toAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

    std::map<std::string, std::vector<std::shared_ptr<Component>>> m_comps;
};

template<typename T>
void EntityManager::AddComponent(const std::shared_ptr<Component>& cmp)
{
    std::cout << "add: " << typeid(T).name() << std::endl;
    m_comps[typeid(T).name()].push_back(cmp);
}

template<typename T>
std::vector<std::shared_ptr<T>> EntityManager::GetComponents()
{
    std::vector<std::shared_ptr<T>> result;
    std::cout << "get: " <<  typeid(T).name() << std::endl;
    for (const auto& cmpPtr : m_comps[typeid(T).name()])
    {
        if (std::dynamic_pointer_cast<T>(cmpPtr))
        {
            result.push_back(std::dynamic_pointer_cast<T>(cmpPtr));
        }
    }

    return result;
}