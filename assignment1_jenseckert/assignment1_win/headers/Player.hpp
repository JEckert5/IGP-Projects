#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "includes.hpp"
#include "Entity.hpp"

using namespace sf;

class Player: public Entity {
public:
	/**
	 * \brief Default constructor. Takes an animation object.
	 * \param a Animation reference
	 */
	Player(Animation& a);

	void tick() override;

    bool readyToFire() const;

    virtual void resetFireTimer();

	/**
	 * \brief Applies damage 'dmg' to player.
	 * \return health remaining.
	 */
	unsigned int damage(unsigned int dmg);

	virtual void reset();

    int health;
    Vector2f tSize;
    std::function<Vector2f(Vector2f, float, float, bool)> bulletFunc;

protected:
    float fireTimer = -0.1f;
};

#endif