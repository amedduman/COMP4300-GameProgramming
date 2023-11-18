#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "Entity.h"

class EntityManager
{
    typedef std::vector<std::shared_ptr<Entity>> EntityVec;
    typedef std::map <std::string, EntityVec> EntityMap;

public:
    EntityManager();
    void Update(); // removes entities that being destroyed last frame
    std::shared_ptr<Entity> AddEntity(const std::string& tag = "default");
    const EntityVec& GetEntities() const;
    const EntityVec& GetEntities(const std::string& tag);
private:
    EntityVec m_entities;
    EntityVec m_toAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0; // number of total entities that being created this will be as unique id for objects
};

