#include "EntityManager.h"

#include <iostream>

#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

EntityManager::EntityManager()
{
}

std::shared_ptr<Entity> EntityManager::AddEntity(const std::string& tag)
{
    auto e = std::make_shared<Entity>(m_totalEntities++, tag);
    m_toAdd.push_back(e);
    return e;
}

void EntityManager::Update()
{
    for (auto& e : m_toAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->Tag()].push_back(e);
    }

    m_toAdd.clear();

    for (auto& pair : m_entityMap)
    {
        EntityVec& entityVec = pair.second;
        entityVec.erase(std::remove_if(entityVec.begin(), entityVec.end(),
                                       [](const std::shared_ptr<Entity> &e)
                                       { return e->IsAlive(); }),
                        entityVec.end());
    }

    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
                                    [](const std::shared_ptr<Entity> &e)
                                    { return e->IsAlive(); }),
                     m_entities.end());
}

const EntityVec& EntityManager::GetEntities() const
{
    return m_entities;
}

const EntityVec& EntityManager::GetEntities(const std::string &tag)
{
    return m_entityMap[tag];
}

void EntityManager::AddComponent(const std::shared_ptr<Component>& cmp)
{
    m_comps[cmp->cmpName].push_back(cmp);
}