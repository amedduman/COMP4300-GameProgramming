#pragma once
#include "../math/Vec2.h"
#include <string>
#include <SFML/Graphics.hpp>

#include "Entity.h"

class Component
{
public:
    std::shared_ptr<Entity> entity;
    std::string cmpName;
    explicit Component(const std::shared_ptr<Entity>& _enity, const std::string& name)
        :entity(_enity),
         cmpName(name)
    {
    }
    virtual ~Component() = default;
};

class CTransform : public Component
{
public:
    // Vec2 pos = Vec2(0,0);

    CTransform(const std::shared_ptr<Entity>& entity, const Vec2& p = Vec2(0, 0)) : Component(entity, typeid(CTransform).name())
    {
        m_pos = p;
        m_previousPos = p;
    }
    Vec2 GetPos()
    {
        return m_pos;
    }
    Vec2 GetPreviousPos()
    {
        return m_previousPos;
    }
    void SetPos(const float x, const float y)
    {
        SetPos(Vec2(x, y));
    }
    void SetPos(const Vec2& pos)
    {
        m_previousPos = m_pos;
        m_pos = pos;
    }
private:
    Vec2 m_pos;
    Vec2 m_previousPos;
};

class Animation
{
public:
    Animation(const sf::Texture& in_texture, const std::vector<sf::IntRect>& in_frames, int in_animSpeed, int in_currentFrame, int in_frameCount)
    {
        texture = in_texture;
        frames = in_frames;
        animSpeed = in_animSpeed;
        currentFrame = in_currentFrame;
        frameCount = in_frameCount;
    }
    Animation()
    {

    }
    void Play(sf::Sprite& sprite)
    {
        gameFrameCount++;
        if(animSpeed < gameFrameCount)
        {
            sprite.setTexture(texture);
            currentFrame++;
            currentFrame %= frameCount;
            std::cout << currentFrame << std::endl;
            sprite.setTextureRect(frames[currentFrame]);
            gameFrameCount = 0;
        }
    }
private:
    sf::Texture texture;
    std::vector<sf::IntRect> frames;
    int currentFrame;
    int frameCount;
    int animSpeed;
    int gameFrameCount;
};

class CSprite : public Component
{
public:
    CSprite(const std::shared_ptr<Entity>& entity, const sf::Texture& in_texture, const sf::IntRect rect, const Vec2 origin) : Component(entity, typeid(CSprite).name())
    {
        sprite.setTexture(in_texture);
        sprite.setOrigin(origin.x, origin.y);
        sprite.setTextureRect(rect);
    }

    void AddAnimation(const sf::Texture& in_texture, const std::vector<sf::IntRect>& in_frames, const std::string& animName, int animSpeed)
    {
        animations[animName] = Animation(in_texture, in_frames, animSpeed, 0, static_cast<int>(in_frames.size()));
    }

    void PLayAnimation(const std::string& animName)
    {
        animations[animName].Play(sprite);
    }

    sf::Sprite& GetSprite()
    {
        return sprite;
    }

    void SetPos(Vec2 pos)
    {
        sprite.setPosition(pos.x, pos.y);
    }
private:
    sf::Sprite sprite;
    // std::vector<sf::IntRect> frames;
    // std::vector<Animation> animations;
    std::map<std::string, Animation> animations;
};

class CShape : public Component
{
public:
    sf::RectangleShape rect;

    CShape(const std::shared_ptr<Entity>& entity, sf::Vector2f size, const sf::Color& fill , const sf::Color& outline = sf::Color::White, float thickness = 1) : Component(entity, typeid(CShape).name())
    {
        rect.setSize(size);
        rect.setFillColor(fill);
        rect.setOrigin(size.x/2, size.y/2);
        rect.setOutlineThickness(thickness);
        rect.setOutlineColor(outline);
    }
};

class CBoundingBox : public Component
{
public:
    float width;
    float height;
    float halfWidth;
    float halfHeight;
    CBoundingBox(const std::shared_ptr<Entity>& entity, const Vec2& size) : Component(entity, typeid(CBoundingBox).name())
    {
        width = size.x;
        halfWidth = width / 2;
        height = size.y;
        halfHeight = height / 2;

    }
;};

class CMarioInput : public Component
{
public:
    CMarioInput(const std::shared_ptr<Entity>& entity) : Component(entity, typeid(CMarioInput).name())
    {}

    bool moveUP = false;
    bool moveDown = false;
    bool moveRight = false;
    bool moveLeft = false;
    bool jump = false;
    bool fire = false;
};

class CVelocity : public Component
{
public:
    Vec2 velocity;
    explicit CVelocity(const std::shared_ptr<Entity>& entity, Vec2 vel = Vec2(0,0)) : Component(entity, typeid(CVelocity).name())
    {
        velocity = vel;
    }
};

class CGravity : public Component
{
public:
    CGravity(const std::shared_ptr<Entity>& entity, const float gravity) : Component(entity, typeid(CGravity).name())
    {
        m_gravity = gravity;
    }
    float GetGravity()
    {
        return m_gravity;
    }
private:
    float m_gravity = 9;
};

// class CScore
// {
// public:
//     int score;

//     CScore(){}

//     CScore(int s)
//             : score(s)
//     {}
// };

// class CLifeSpan
// {
// public:
//     int remaining;
//     int total;

//     CLifeSpan(){}

//     CLifeSpan(int total)
//             : remaining(total), total(total)
//     {}
// };

// class CInput
// {
// public:
//     bool up =false;
//     bool down = false;
//     bool left = false;
//     bool right = false;
//     bool leftMouse = false;
//     bool rightMouse = false;
//     CInput() {}
// };

// class CCollision
// {
// public:
//     float radius;

//     CCollision(float r)
//             : radius (r){}
// };
