#pragma once
#include <string>
#include "Components.h"

typedef std::tuple<
    CTransform,
    CShape,
    CLifeSpan,
    CInput
    >ComponentTuple;

class Entity
{
public:
    Entity(size_t id, const std::string &tag);

    void Destroy();
    bool IsAlive() const;
    const std::string &Tag() const;
    size_t Id() const;

    template <typename T>
    bool HasComp() const
    {
        return GetComp<T>().has;
    }

    template <typename T, typename... TArgs>
    T& AddComp(TArgs &&...mArgs)
    {
        auto &comp = GetComponent<T>();
        comp = T(std::forward<TArgs>(mArgs)...);
        comp.has = true;
        return comp;
    }

    template <typename T>
    T& GetComp()
    {
        return std::get<T>(m_comps);
    }

    template <typename T>
    const T& GetComp() const
    {
        return std::get<T>(m_comps);
    }

    template <typename T>
    void RemoveComp()
    {
        GetComp<T>() = T();
    }

private:
    bool m_isAlive = true;
    size_t m_id = 0;
    std::string m_tag = "default";
    ComponentTuple m_comps;
};
