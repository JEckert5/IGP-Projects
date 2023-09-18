#include "../headers/Dio.hpp"

// extern float DeltaTime;
// extern std::mt19937 m;

Dio::Dio(Animation& a, const int& h, std::list<Entity*>* ptr): Player(a), entityPtr(ptr) {
    scale.x = 0.3f;
    scale.y = 0.3f;
    anim.sprite.setScale(scale);
    name = "Dio";
    health = h;
}

Dio::~Dio() {
    entityPtr = nullptr;
    delete entityPtr;
}

void Dio::tick() {
    // Randomize attack pattern
    if (readyToFire() && !firing) {
        resetFireTimer();
        firing = true;
        currentPattern = m() % 4 + 1;
    } else if (firing) {
        switch (currentPattern) {
        case 1:
            patternOne();
            break;
        case 2:
            patternTwo();
            break;
        case 3:
            patternThree();
            break;
        case 4:
            patternFour();
            break;
        default:
            break;
        }

        if (pulseCounter > pulseCounts[currentPattern - 1]){
            pulseCounter = 0;
            firing = false;
        }
    }

    // Randomize movement if timer done
    if (moveTimer <= 0) {
        moveTimer = m() % 4 + 1;
        if (m() % 500 > 250) {
            moveDir = 1;
        } else {
            moveDir = -1;
        }
    } else
        moveTimer -= DeltaTime;

    // Don't go out of the screen bounds
    if (pos.x <= tSize.x / 2) {
        moveTimer = m() % 4 + 1;
        moveDir = 1;
    } else if (pos.x >= Width - tSize.x / 2) {
        moveTimer = m() % 4 + 1;
        moveDir = -1;
    }

    // Move and update
    pos.x += moveDir * speed * DeltaTime;
    anim.sprite.setPosition(pos);
    fireTimer = fireTimer > -0.1f ? fireTimer - DeltaTime : fireTimer;
}

void Dio::config(Animation& a, const Animation& bAnim, Vector2f s, Vector2f p, float r) {
    Entity::config(a, s, p, r);
    tSize = static_cast<Vector2f>(anim.sprite.getTexture()->getSize()) * scale.x;
    bulletAnim = bAnim;
}

void Dio::resetFireTimer() {
    fireTimer = 1.5f;
}

void Dio::patternOne() {
    pulseTimer -= DeltaTime;

    if (pulseTimer <= 0.f) {
        auto db = new Bullet(bulletAnim, pos, DioBulletDmg, DioBulletVel, this);
        db->config(Bullet::SchlerpType::NONE, [](Vector2f v2, float t, float v, bool d) {
            v2.y += DeltaTime * v;
            return v2;
        });

        entityPtr->push_back(db);
        db = nullptr;

        db = new Bullet(bulletAnim, pos, DioBulletDmg, DioBulletVel, this);
        db->config(Bullet::SchlerpType::NONE, [](Vector2f v2, float t, float v, bool d) {
            v2.x += DeltaTime * v;
            v2.y += DeltaTime * v;
            return v2;
        });

        db->anim.sprite.setRotation(135);
        entityPtr->push_back(db);
        db = nullptr;

        db = new Bullet(bulletAnim, pos, DioBulletDmg, DioBulletVel, this);
        db->config(Bullet::SchlerpType::NONE, [](Vector2f v2, float t, float v, bool d) {
            v2.x -= DeltaTime * v;
            v2.y += DeltaTime * v;
            return v2;
        });
        db->anim.sprite.setRotation(225);
        entityPtr->push_back(db);
        db = nullptr;

        delete db;

        pulseTimer = 0.05f;
        pulseCounter += 1;
    } 
}

void Dio::patternTwo() {
    pulseTimer -= DeltaTime;

    if (pulseTimer <= 0.f) {
        auto center = new Entity;

        center->pos = pos;
        center->setTick([center] {
            center->pos.y += DeltaTime * DioBulletVel;
        });

        entityPtr->push_back(center);

        for (int i = 0; i < 9; i++) {
            auto db = new Bullet(bulletAnim, pos, DioBulletDmg, 0, this);
            db->config(Bullet::SchlerpType::INF, [center, i](Vector2f v2, float t, float v, bool d) {
                // Calculate angle of current projectile.
                const auto angle = static_cast<float>(i * 40) + t;
                // Distance from center.
                const float dist = t * 50;
                // Don't need DeltaTime as it's already in centers tick function.
                // https://stackoverflow.com/questions/43641798/how-to-find-x-and-y-coordinates-on-a-flipped-circle-using-javascript-methods
                // I may be stupid.
                v2.x = center->pos.x + dist * cosf(angle); //* DeltaTime;
                v2.y = center->pos.y + dist * sinf(angle); //* DeltaTime;
                return v2;
            });

            entityPtr->push_back(db);
            db = nullptr;
        }

        pulseTimer = 0.35f;
        pulseCounter += 1;
    }
}

void Dio::patternThree() {
    pulseTimer -= DeltaTime;

    if (pulseTimer <= 0.f) {
    	auto db = new Bullet(bulletAnim, pos, DioBulletDmg, DioBulletVel, this);
        db->config(Bullet::SchlerpType::LERP, [](Vector2f v2, float t, float v, bool d) {
            // if interpolation direction (d) is true we go up, if not down.
            v2.x += (d ? lerp(0, 200, t) : -lerp(0, 200, t)) * DeltaTime;
            v2.y += v * DeltaTime;
            return v2;
        });

        entityPtr->push_back(db);

        pulseTimer = 0.1f;
        pulseCounter += 1;
    }
}

void Dio::patternFour() {
    // << "Woah4\n" << std::flush;
    pulseTimer -= DeltaTime;

    if (pulseTimer <= 0.f) {
	    auto db = new Bullet(bulletAnim, pos, DioBulletDmg, DioBulletVel, this);
        db->config(Bullet::SchlerpType::LERP, [](Vector2f v2, float t, float v, bool d) { 
            v2.y += (d ? lerp(0, 300, t) : -lerp(0, 100, t)) * DeltaTime;
            return v2;
        });

        entityPtr->push_back(db);

        pulseTimer = 0.185f;
        pulseCounter += 1;
    }
}

void Dio::reset() {
    pos = Vector2f(Width / 2, Height / 4);
    markedForNegation = false;
    health = 1000;
    resetFireTimer();
    pulseCounter = 0;
    currentPattern = 0;
    firing = false;
}