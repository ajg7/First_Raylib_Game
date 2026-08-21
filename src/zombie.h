#pragma once

#include "raylib.h"
#include "animation.h"
#include "config.h"
#include "layer.h"

struct Zombie {
    Vector2 position;
    Texture2D walkTexture;
    Texture2D deadTexture;
    Animation walkAnim;
    Animation deadAnim;
    int hp;
    bool dead{false};
    bool facingLeft{false};

    Zombie(const Vector2 startPosition, const Texture2D walk, const Texture2D deadTex,
           const Animation walkAnimation, const Animation deadAnimation, const int startHp)
        : position(startPosition), walkTexture(walk), deadTexture(deadTex),
          walkAnim(walkAnimation), deadAnim(deadAnimation), hp(startHp) {}

    static Zombie load(const Vector2 startPosition) {
        return Zombie{
            startPosition,
            LoadTexture(Config::Assets::Zombie::walkPath),
            LoadTexture(Config::Assets::Zombie::deadPath),
            Animation{Config::Assets::Zombie::walkFrameCount, Config::Assets::Zombie::walkFrameDuration},
            Animation{Config::Assets::Zombie::deadFrameCount, Config::Assets::Zombie::deadFrameDuration},
            Config::Assets::Zombie::startHp
        };
    }

    ~Zombie() {
        UnloadTexture(walkTexture);
        UnloadTexture(deadTexture);
    }

    Zombie(const Zombie&) = delete;
    Zombie& operator=(const Zombie&) = delete;
    Zombie(Zombie&&) = delete;
    Zombie& operator=(Zombie&&) = delete;

    Rectangle bounds() const {
        return Rectangle{
            position.x, position.y,
            static_cast<float>(Config::Assets::Zombie::frameWidth),
            static_cast<float>(Config::Assets::Zombie::frameHeight)
        };
    }

    Rectangle expandedBounds(const float halfWidth, const float halfHeight) const {
        const Rectangle box = bounds();
        return Rectangle{
            box.x - halfWidth, box.y - halfHeight,
            box.width + 2.0f * halfWidth, box.height + 2.0f * halfHeight
        };
    }
    
    void takeHit() {
        if (dead) return;
        --hp;
        if (hp <= 0) {
            dead = true;
            deadAnim.reset();
        }
    }

    void update(const double simDt, const float targetX) {
        if (dead) {
            if (deadAnim.frameIndex != Config::Assets::Zombie::deadFrameCount - 1) {
                deadAnim.advance(simDt);
            }
            return;
        }

        const float direction = (targetX < position.x) ? -1.0f : 1.0f;
        facingLeft = direction < 0.0f;
        position.x += direction * Config::Assets::Zombie::moveSpeed * static_cast<float>(simDt);

        walkAnim.advance(simDt);
    }

    void draw() const {
        const Texture2D& texture = dead ? deadTexture : walkTexture;
        const Animation& anim = dead ? deadAnim : walkAnim;

        const Rectangle src = Layer::frameRect(anim, Config::Assets::Zombie::frameWidth, Config::Assets::Zombie::frameHeight, facingLeft);
        const Rectangle dst{
            position.x, position.y,
            static_cast<float>(Config::Assets::Zombie::frameWidth),
            static_cast<float>(Config::Assets::Zombie::frameHeight)
        };
        DrawTexturePro(texture, src, dst, {}, 0.0f, WHITE);
    }
};
