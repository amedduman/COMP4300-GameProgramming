#include "Entity.h"

Entity::Entity(size_t id, const std::string& tag)
                : m_id(id), m_tag(tag)
{}

bool Entity::IsMarkedToBeDestroyedForNextFrame() const
{
    return false;
}

const std::string& Entity::Tag() const
{
    return "default";
}

const size_t Entity::Id() const
{
    return 0;
}

void Entity::Destroy()
{

}
