#pragma once

#include "raylib.h"
#include "animation.h"
#include "config.h"
#include "layer.h"

struct Player {
    Vector2 position;
    Texture2D idleTexture;
    Texture2D walkTexture;
    Animation idleAnim;
    Animation walkAnim;
    bool facingLeft{false};
    bool wasMoving{false};

    Player(const Vector2 startPosition, const Texture2D idle, const Texture2D walk, const Animation idleAnimation, const Animation walkAnimation)
        : position(startPosition), idleTexture(idle), walkTexture(walk), idleAnim(idleAnimation), walkAnim(walkAnimation) {}

    static Player load(const Vector2 startPosition) {
        return Player{
            startPosition,
            LoadTexture(Config::Assets::Scientist::idlePath),
            LoadTexture(Config::Assets::Scientist::walkPath),
            Animation{Config::Assets::Scientist::idleFrameCount, Config::Assets::Scientist::idleFrameDuration},
            Animation{Config::Assets::Scientist::walkFrameCount, Config::Assets::Scientist::walkFrameDuration}
        };
    }

    ~Player() {
        UnloadTexture(idleTexture);
        UnloadTexture(walkTexture);
    }

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) = delete;
    Player& operator=(Player&&) = delete;

    void update(const double simDt) {
        const bool movingLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
        const bool movingRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
        const bool moving = movingLeft != movingRight;

        if (movingLeft) facingLeft = true;
        if (movingRight) facingLeft = false;

        if (moving) {
            if (!wasMoving) walkAnim.reset();
            walkAnim.advance(simDt);
            position.x += (movingLeft ? -1.0f : 1.0f) * Config::Assets::Scientist::moveSpeed * static_cast<float>(simDt);
        } else {
            if (wasMoving) idleAnim.reset();
            idleAnim.advance(simDt);
        }

        wasMoving = moving;
    }

    void draw() const {
        const Texture2D& texture = wasMoving ? walkTexture : idleTexture;
        const Animation& anim = wasMoving ? walkAnim : idleAnim;

        const Rectangle src = Layer::frameRect(
            anim,
            Config::Assets::Scientist::frameWidth,
            Config::Assets::Scientist::frameHeight,
            facingLeft
        );
        const Rectangle dst{
            position.x,
            position.y,
            static_cast<float>(Config::Assets::Scientist::frameWidth),
            static_cast<float>(Config::Assets::Scientist::frameHeight)
        };

        DrawTexturePro(texture, src, dst, {}, 0.0f, WHITE);
    }
};
