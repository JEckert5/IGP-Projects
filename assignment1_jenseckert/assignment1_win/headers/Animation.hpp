#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "includes.hpp"

using namespace sf;

class Animation {
public:
    Animation() = default;

    /**
     * \brief Copy Constructor.
     */
    Animation(const Animation& other);

    /**
     * \brief Constructor that actually constructs
     * \param t Texture
     * \param x Origin x
     * \param y Origin y
     * \param w Width of each sprite tile
     * \param h Height of each sprite tile
     * \param count frame count
     * \param speed Time the animation takes(in seconds).
     */
    Animation(Texture& t, int x, int y, int w, int h, int count, float speed);

    void update();

    Sprite sprite;
 
private:
    std::vector<IntRect> mFrames;
    float mFrame = 0, mFps = 0;
};

#endif