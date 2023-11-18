#include "Entity.h"

Entity::Entity(size_t id, const std::string &tag)
    : m_id(id), m_tag(tag)
{ 
}

bool Entity::IsAlive() const
{
    return m_isAlive;
}

const std::string &Entity::Tag() const
{
    return m_tag;
}

size_t Entity::Id() const
{
    return m_id;
}

void Entity::Destroy()
{
    m_isAlive = true;
}
