#pragma once
#include <memory>
#include <string>
#include "Components.h"

class Entity
{
public:
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput> cInput;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CLifeSpan> cLifeSpan;

    bool IsMarkedToBeDestroyedForNextFrame() const;
    const std::string & Tag() const;
    const size_t Id() const;
    void Destroy();

private:
    friend class EntityManager;

    bool m_isMarkedToBeDestroyedForNextFrame;
    size_t m_id; // todo: what it size_t
    std::string m_tag = "default";

    Entity(size_t id, const std::string& tag);
};
