#ifndef BULLET_HPP
#define BULLET_HPP

#include "includes.hpp"
#include "Player.hpp"

const Vector2f BulletScale(0.6f, 0.6f);
constexpr float BulletRadius = 10.f;

class Bullet: public Entity {
public:
    enum class SchlerpType {
        INF = 1,
        LERP,
        NONE,
    };

    Bullet(Animation& a, 
           Vector2f pos, 
           float pow, 
           float v, 
           Entity* s, 
           float radius = BulletRadius);

    // Shadows on purpose. There's certainly a better way but I don't care rn.
    void config(SchlerpType st, std::function<Vector2f(Vector2f, float, float, bool)> func);

    void tick() override;

    bool collision(Entity* other) override;

private:
    std::function<Vector2f(Vector2f, float, float, bool)> interpFunc;
    float schlerp = 0.001f;
    float velocity, power;
    bool schlerpDir = true;
    SchlerpType sType;
    Entity* source;
};

#endif