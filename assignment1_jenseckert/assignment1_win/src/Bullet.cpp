#include "../headers/Bullet.hpp"

// extern float DeltaTime;

Bullet::Bullet(Animation& a, 
               Vector2f pos, 
               float pow, 
               float v, 
               Entity* s, 
               float radius): 
    sType(SchlerpType::NONE), velocity(v), power(pow) {

    scale = BulletScale;
    anim = a;
    this->pos = pos;
    hitRadius = radius;
    markedForNegation = false;
    source = s;

    // Default movement lambda.
    interpFunc = [](Vector2f v2, float t, float v, bool d) {
        return v2;
    };
}

void Bullet::config(Bullet::SchlerpType st, std::function<Vector2f(Vector2f, float, float, bool)> func) {
    interpFunc = std::move(func);
    sType = st;
}

void Bullet::tick() {
    pos = interpFunc(pos, schlerp, velocity, schlerpDir);
    
    switch (sType) {
    case SchlerpType::INF:
        schlerp += DeltaTime;
    	break;
    case SchlerpType::LERP:
        schlerp = schlerpDir ? schlerp + DeltaTime : schlerp - DeltaTime;
        schlerp = std::clamp<float>(schlerp, 0.f, 1.f);

        if (0.f == schlerp)
            schlerpDir = true;
        else if (schlerp == 1.f)
            schlerpDir = false;

        break;
    case SchlerpType::NONE:
        break;
    }

    anim.update();
    anim.sprite.setPosition(pos);
    if (pos.y < 0 || pos.y > Height || pos.x < 0 || pos.x > Width)
        markedForNegation = true;
}

bool Bullet::collision(Entity* other) {
    if (other == source)
        return false;

    if (Entity::collision(other)) {
        if (other->name == "Dio" || other->name == "Player") {
            dynamic_cast<Player*>(other)->damage(power);
            markedForNegation = true;
        }
        return true;
    }

    return false;
}