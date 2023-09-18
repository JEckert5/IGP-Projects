#include "../headers/includes.hpp"
#include "../headers/Entity.hpp"

Entity::Entity(const Entity& other) {
    name = other.name;
    pos = other.pos;
    scale = other.scale;
    anim = other.anim;
    speed = other.speed;
    hitRadius = other.hitRadius;
}

void Entity::config(Animation& a, Vector2f s, Vector2f p, float r) {
    anim = a;
    pos = p;
    hitRadius = r;
    scale = s;
    anim.sprite.setScale(scale);
    anim.sprite.setPosition(p);
}

void Entity::setTick(std::function<void()> t) {
    optionalTick = std::move(t);
}

void Entity::tick() { 
    anim.update();
    if (optionalTick)
        optionalTick();
}

void Entity::draw(RenderWindow& window) {
    window.draw(anim.sprite);
}

bool Entity::collision(Entity* other) {
    if (other == this)
        return false;
    return (other->pos.x - pos.x) * (other->pos.x - pos.x) +
        (other->pos.y - pos.y) * (other->pos.y - pos.y) <
        (hitRadius + other->hitRadius) * (hitRadius + other->hitRadius);
}