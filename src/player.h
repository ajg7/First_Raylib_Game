#pragma once

#include "raylib.h"
#include "animation.h"
#include "config.h"
#include "layer.h"
#include "bullet.h"
#include <algorithm>
#include <optional>

struct Player {
    Vector2 position;
    Texture2D idleTexture;
    Texture2D walkTexture;
    Animation idleAnim;
    Animation walkAnim;
    bool facingLeft{false};
    bool wasMoving{false};
    Texture2D specialTexture;
    Animation specialAnim;
    bool shooting{false};
    Texture2D crouchFireTexture;
    Animation crouchFireAnim;
    bool crouching{false};
    bool bulletSpawnedThisShot{false};
    double fireCooldownRemaining{0.0};

    Player(const Vector2 startPosition, const Texture2D idle, const Texture2D walk, const Texture2D special,
           const Texture2D crouchFire, const Animation idleAnimation, const Animation walkAnimation,
           const Animation specialAnimation, const Animation crouchFireAnimation)
        : position(startPosition), idleTexture(idle), walkTexture(walk), specialTexture(special),
          crouchFireTexture(crouchFire), idleAnim(idleAnimation), walkAnim(walkAnimation),
          specialAnim(specialAnimation), crouchFireAnim(crouchFireAnimation) {}

    static Player load(const Vector2 startPosition) {
        return Player{
            startPosition,
            LoadTexture(Config::Assets::Scientist::idlePath),
            LoadTexture(Config::Assets::Scientist::walkPath),
            LoadTexture(Config::Assets::Scientist::specialPath),
            LoadTexture(Config::Assets::Scientist::crouchFirePath),
            Animation{Config::Assets::Scientist::idleFrameCount, Config::Assets::Scientist::idleFrameDuration},
            Animation{Config::Assets::Scientist::walkFrameCount, Config::Assets::Scientist::walkFrameDuration},
            Animation{Config::Assets::Scientist::specialFrameCount, Config::Assets::Scientist::specialFrameDuration},
            Animation{Config::Assets::Scientist::crouchFireFrameCount, Config::Assets::Scientist::crouchFireFrameDuration}
        };
    }

    ~Player() {
        UnloadTexture(idleTexture);
        UnloadTexture(walkTexture);
        UnloadTexture(specialTexture);
        UnloadTexture(crouchFireTexture);
    }

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) = delete;
    Player& operator=(Player&&) = delete;

    std::optional<Bullet> update(const double simDt) {
        const bool movingLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
        const bool movingRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
        const bool moving = movingLeft != movingRight;
        const bool crouchInput = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);

        if (movingLeft) facingLeft = true;
        if (movingRight) facingLeft = false;

        if (moving) {
            const float delta = (movingLeft ? -1.0f : 1.0f) * Config::Assets::Scientist::moveSpeed * static_cast<float>(simDt);
            const float minX = static_cast<float>(Config::World::ArenaMinX);
            const float maxX = static_cast<float>(Config::World::ArenaMaxX - Config::Assets::Scientist::frameWidth);
            position.x = std::clamp(position.x + delta, minX, maxX);
        }

        if (fireCooldownRemaining > 0.0) fireCooldownRemaining -= simDt;

        if (!shooting && !crouching && crouchInput && IsKeyDown(KEY_SPACE) && fireCooldownRemaining <= 0.0) {
            crouching = true;
            bulletSpawnedThisShot = false;
            crouchFireAnim.reset();
        } else if (!shooting && !crouching && IsKeyDown(KEY_SPACE) && fireCooldownRemaining <= 0.0) {
            shooting = true;
            bulletSpawnedThisShot = false;
            specialAnim.reset();
        }

        std::optional<Bullet> spawned;

        if (crouching) {
            const int previousFrame = crouchFireAnim.frameIndex;
            crouchFireAnim.advance(simDt);

            if (!bulletSpawnedThisShot && crouchFireAnim.frameIndex == Config::Assets::Scientist::crouchFireFlashFrame) {
                bulletSpawnedThisShot = true;
                const float dstY = position.y + static_cast<float>(Config::Assets::Scientist::frameHeight - Config::Assets::Scientist::crouchFireFrameHeight);
                const float muzzleX = position.x + (facingLeft ? 0.0f : static_cast<float>(Config::Assets::Scientist::crouchFireFrameWidth));
                const float muzzleY = dstY + static_cast<float>(Config::Assets::Scientist::crouchFireFrameHeight) * 0.45f;
                const float dir = facingLeft ? -1.0f : 1.0f;
                spawned = Bullet{
                    Vector2{muzzleX, muzzleY},
                    Vector2{dir * Config::Gameplay::bulletSpeed, 0.0f}
                };
            }

            if (previousFrame == Config::Assets::Scientist::crouchFireFrameCount - 1 && crouchFireAnim.frameIndex == 0) {
                crouching = false;
                fireCooldownRemaining = Config::Assets::Scientist::fireCooldown;
                idleAnim.reset();
                walkAnim.reset();
            }
        } else if (shooting) {
            const int previousFrame = specialAnim.frameIndex;
            specialAnim.advance(simDt);

            if (!bulletSpawnedThisShot && specialAnim.frameIndex == Config::Assets::Scientist::specialFlashFrame) {
                bulletSpawnedThisShot = true;
                const float muzzleX = position.x + (facingLeft ? 0.0f : static_cast<float>(Config::Assets::Scientist::frameWidth));
                const float muzzleY = position.y + static_cast<float>(Config::Assets::Scientist::frameHeight) * 0.57f;
                const float dir = facingLeft ? -1.0f : 1.0f;
                spawned = Bullet{
                    Vector2{muzzleX, muzzleY},
                    Vector2{dir * Config::Gameplay::bulletSpeed, 0.0f}
                };
            }

            if (previousFrame == Config::Assets::Scientist::specialFrameCount - 1 && specialAnim.frameIndex == 0) {
                shooting = false;
                fireCooldownRemaining = Config::Assets::Scientist::fireCooldown;
                idleAnim.reset();
                walkAnim.reset();
            }
        } else if (moving) {
            if (!wasMoving) walkAnim.reset();
            walkAnim.advance(simDt);
        } else {
            if (wasMoving) idleAnim.reset();
            idleAnim.advance(simDt);
        }

        wasMoving = moving;
        return spawned;
    }

    void draw() const {
        const Texture2D* texture = &idleTexture;
        const Animation* anim = &idleAnim;
        int frameW = Config::Assets::Scientist::frameWidth;
        int frameH = Config::Assets::Scientist::frameHeight;

        if (crouching) {
            texture = &crouchFireTexture;
            anim = &crouchFireAnim;
            frameW = Config::Assets::Scientist::crouchFireFrameWidth;
            frameH = Config::Assets::Scientist::crouchFireFrameHeight;
        } else if (shooting) {
            texture = &specialTexture;
            anim = &specialAnim;
        } else if (wasMoving) {
            texture = &walkTexture;
            anim = &walkAnim;
        }

        const float dstY = position.y + static_cast<float>(Config::Assets::Scientist::frameHeight - frameH);
        const Rectangle src = Layer::frameRect(*anim, frameW, frameH, facingLeft);
        const Rectangle dst{
            position.x, dstY,
            static_cast<float>(frameW),
            static_cast<float>(frameH)
        };
        DrawTexturePro(*texture, src, dst, {}, 0.0f, WHITE);
    }
};
