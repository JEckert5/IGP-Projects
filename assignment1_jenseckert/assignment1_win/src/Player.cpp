#include "../headers/Player.hpp"

Player::Player(Animation& a) {
    pos.y = Height - a.sprite.getTexture()->getSize().y;
    scale = Vector2f(0.4f, 0.4f);
    anim = a;
    anim.sprite.setScale(scale);
    pos.x = Width / 4;
    tSize = static_cast<Vector2f>(anim.sprite.getTexture()->getSize()) * scale.x;
    anim.sprite.setPosition(pos);
    name = "Player";
    health = 100;
    hitRadius = 50;

    bulletFunc = [](Vector2f v2, float t, float v, bool d) -> Vector2f {
        v2.y -= v * DeltaTime;
        return v2;
    };
}

void Player::tick() {
    anim.update();
    if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
        pos.x -= speed * DeltaTime;
    if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
        pos.x += speed * DeltaTime;
    if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
        pos.y -= speed * DeltaTime;
    if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
        pos.y += speed * DeltaTime;

    pos.x = std::clamp<float>(pos.x, tSize.x *1.5f, static_cast<float>(Width) - (tSize.x * 1.5f));
    pos.y = std::clamp<float>(pos.y, tSize.y * 5.f, static_cast<float>(Height) - (tSize.y * 1.5f));

    anim.sprite.setPosition(pos);
    fireTimer = fireTimer > -0.1f ? fireTimer - DeltaTime : fireTimer;

    //std::cout << "x: " << pos.x << ", y: " << pos.y << std::endl;
    // std::cout << DeltaTime << std::endl;
}

bool Player::readyToFire() const {
    return fireTimer < 0;
}

void Player::resetFireTimer() {
    fireTimer = 0.12f;
}

unsigned int Player::damage(unsigned int dmg) {
    health -= dmg;

    if (health <= 0)
        markedForNegation = true;

    return health;
}

void Player::reset() {
    pos.y = Height - tSize.y;
    pos.x = Width / 4;
    markedForNegation = false;
    resetFireTimer();
    health = 100;
}