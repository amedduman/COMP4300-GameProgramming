#pragma once
#include <SFML/Graphics.hpp>
#include "../../engine/ecs/EntityManager.h"

class MarioGame
{
public:
    MarioGame();
    void Run();
private:
    sf::RenderWindow m_window;
    EntityManager m_entityManager;
    int m_frameCount = 0;
    int m_countOfJumpInputPressed = 0;

    void SpawnPlayer();
    void SpawnTiles();
    void SpawnBullet();
    Vec2 RectVsRect(const std::shared_ptr<CBoundingBox>& rect1, const std::shared_ptr<CBoundingBox>& rect2, bool doForPreviousPos = false);
    void Reselotion();
    void Reselotion(std::shared_ptr<CBoundingBox> bb);
    void SyncShapeAndTransform();
    void SyncSpriteAndTransform();
    void MoveCamera();

    void SInput();
    void SCalculateVelocity();
    void SMovement();
    void SApplyGravity();
    void SDetectCollision();
    void SAnimate();
    void SRender();

    sf::Texture playerRunTex;
    sf::Texture playerIdleTex;
    sf::Texture tilemapTex;
    sf::Texture hazardTex;
    sf::Texture bulletTex;
    sf::Texture specialTileTex;
};
