#include "EntityManager.h"
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map <std::string, EntityVec> EntityMap;

EntityManager::EntityManager()
{

}

std::shared_ptr<Entity> EntityManager::AddEntity(const std::string &tag)
{
    auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
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
                                        [](const std::shared_ptr<Entity>& e) { return e->IsMarkedToBeDestroyedForNextFrame(); }),
                        entityVec.end());
    }

    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
                     [](const std::shared_ptr<Entity>& e) { return e->IsMarkedToBeDestroyedForNextFrame(); }),
                     m_entities.end());

}

const EntityVec& EntityManager::GetEntities() const
{
    return m_entities;
}
