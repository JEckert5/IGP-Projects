#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "includes.hpp"
#include "Animation.hpp"

using namespace sf;

class Entity {
public:
    Entity() = default;

    /**
     * \brief Copy constructor
     */
    Entity(const Entity& other);

    /**
     * \brief Configure various parameters of the Entity. Can be overloaded.
     * \param a Animation
     * \param s Scale
     * \param p Initial Position
     * \param r Hit circle radius
     */
    virtual void config(Animation& a, Vector2f s, Vector2f p, float r);

    /**
     * \brief If an Entity needs to be instantiated without a base class, a lambda can be passed as the tick function.
     * \param t Custom tick lambda.
     */
    virtual void setTick(std::function<void()> t);

    virtual ~Entity() = default;

    virtual void tick();

    /**
     * \brief Default draw function.
     * \param window Reference to the main RenderWindow object
     */
    virtual void draw(RenderWindow& window);

    /**
     * \brief Check for collision with another Entity.
     * \param other Entity to check against.
     * \return Did hit or not.
     */
    virtual bool collision(Entity* other);

    std::string name;
    Vector2f pos, scale;
    float hitRadius, speed = 0;
    bool markedForNegation = false;
    Animation anim;
    std::function<void()> optionalTick;
};

#endif