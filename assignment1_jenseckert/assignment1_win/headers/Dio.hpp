#ifndef DIO_HPP
#define DIO_HPP

#include "includes.hpp"
#include "Player.hpp"
#include "Bullet.hpp"

constexpr float DioBulletVel = 200.f;
constexpr float DioBulletDmg = 5;
// constexpr float BulletRadius = 10;

class Dio final: public Player {
public:
    Dio(Animation& a, const int& h, std::list<Entity*>* ptr);

    ~Dio() override;

    void tick() override;

    void config(Animation& a, const Animation& bAnim, Vector2f s, Vector2f p, float r);

    void resetFireTimer() override;

    /**
     * \brief Shoots three bursts in three directions.
     */
    void patternOne();

    /**
     * \brief Creates a ring of bullets that expand outward while rotating.
     */
    void patternTwo();

    /**
     * \brief Bullets move in a weird bumpy pattern.
     */
    void patternThree();

    /**
     * \brief Bullets move up and down
     */
    void patternFour();

    void reset() override;

private:
    float moveTimer = 0;
    // 1 = right, -1 = left;
    int moveDir = 1;
    std::list<Entity*>* entityPtr;
    Animation bulletAnim;
    bool firing = false;
    float pulseTimer = 0.f;
    int pulseCounts[4] = {5, 1, 6, 3};
    int pulseCounter = 0;
    int currentPattern = 0;
};

#endif