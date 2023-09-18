#include "../headers/Animation.hpp"

Animation::Animation(const Animation& other) {
    sprite = other.sprite;
    mFps = other.mFps;
    mFrame = other.mFrame;
    mFrames = other.mFrames;
}

Animation::Animation(Texture& t, int x, int y, int w, int h, int count, float speed) {
    mFrame = 0;
    mFps = speed;
    for (int i = 0; i < count; i++)
        mFrames.emplace_back(IntRect(x + i * w, y, w, h));
    sprite.setTexture(t);
    sprite.setOrigin(w / 2, h / 2);
    sprite.setTextureRect(mFrames[0]);
}

void Animation::update() {
    mFrame += mFps;
    int n = mFrames.size();
    if (mFrame >= n) mFrame -= n;
    if (n > 0) sprite.setTextureRect(mFrames[static_cast<int>(mFrame)]);
}