#pragma once
#include <vector>
#include <map>
#include <string>
#include "Entity.h"
#include "Components.h"
#include <any>
#include <iostream>

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
    template<typename T> void AddComponent(T cmp);
    template<typename T> std::vector<T>& GetComponents();
private:
    EntityVec m_entities;
    EntityVec m_toAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

    std::map<std::string, std::vector<Component>> m_cmpMap;
};

template<typename T>
void EntityManager::AddComponent(T cmp)
{
    m_cmpMap[typeid(cmp).name()].push_back(cmp);
}

template<typename T>
std::vector<T>& EntityManager::GetComponents()
{
    // std::cout << typeid(T).name() << std::endl;
    return  m_cmpMap[typeid(T).name()];
}