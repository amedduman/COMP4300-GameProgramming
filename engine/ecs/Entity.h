#pragma once
#include <string>

class Entity
{
public:
    Entity(size_t id, const std::string &tag);

    void Destroy();
    bool IsAlive() const;
    const std::string &Tag() const;
    size_t Id() const;
private:
    bool m_isAlive = true;
    size_t m_id = 0;
    std::string m_tag = "default";
};
