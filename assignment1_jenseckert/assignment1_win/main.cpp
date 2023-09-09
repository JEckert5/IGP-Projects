#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <random>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

using namespace sf;

int width = 940, height = 720;

static float deltaTime = 0.0f;

// Ripped from the asteroid demo.
class Animation {
public:
    Animation() = default;

    Animation(Texture& t, int x, int y, int w, int h, int count, float speed) {
        frame = 0;
        fps = speed;

        for (int i = 0; i < count; i++)
            frames.push_back(IntRect(x + i * w, y, w, h));

        sprite.setTexture(t);
        sprite.setOrigin(w / 2, h / 2);
        sprite.setTextureRect(frames[0]);
    }

    void update() {
        frame += fps;
        int n = frames.size();
        if (frame >= n) frame -= n;
        if (n > 0) sprite.setTextureRect(frames[int(frame)]);
    }

    Sprite& getSprite() {
        return sprite;
    }
 
private:
    Sprite sprite;
    std::vector<IntRect> frames;

    float frame, fps;
};

class Entity {
public:
    Entity() = default;

    virtual void tick() { };

    virtual void draw(RenderWindow& window) { }

    float getSpeed() const { return speed; }

    const void setSpeed(float x) { speed = x; }

    std::string name;
    Vector2<float> pos;
    Vector2f scale;

protected:
    Animation anim;
    float speed;
};

class Video: public Entity {
public:
    Video(){//: globlEntities(ent) {
        name = "Video";
    }

    // relies on files having format 'output-XXXX.png' naming format.
    void install(const std::string& frameFolder, int numFrames, float fps, const std::string& audioPath) {
        this->fps = 1 / fps;
        audio.openFromFile(audioPath);
        frame = 0;

        for (int i = 1; i <= numFrames; i++) {
            std::string file;

            if (i < 10) {
                file = "/output-000";
            } else if (i < 100) {
                file = "/output-00";
            } else if (i < 1000) {
                file = "/output-0";
            } else {
                file = "/output-";
            }

            Texture t;
            t.loadFromFile(frameFolder + file + std::to_string(i) + ".png");

            frames.emplace_back(t);
        }

        playing = false;
        accumulator = 0.0f;
    }

    void tick() override {
        accumulator += deltaTime;

        if (accumulator >= fps) {
            accumulator = 0.0f;
            frame += 1;
            sprite.setTexture(frames[frame]);
        }
    }

    void start() {
        audio.play();
        playing = true;
        sprite.setTexture(frames[0]);
    }

    bool isPlaying() {
        return playing;
    }

    void draw(RenderWindow& window) override {
        window.draw(sprite);
    }
private:
    bool playing;
    Sprite sprite;
    std::vector<Texture> frames;
    Music audio;
    int frame;
    float fps, accumulator;
};

class Enemy: public Entity {
public:
    void draw(RenderWindow& window) override {

    }
};

class Bullet: public Entity {
public:
    Bullet(float v, Vector2<float> dir, float p): velocity(v), direction(dir), power(p) {
        scale.x = 0.3f;
        scale.y = 0.3f;
        bt.loadFromFile("images/bullet.gif");
        anim.getSprite().setTexture(bt);
        anim.getSprite().setScale(scale);
        name = "Bullet";
        pos = dir;
        markedForNegation = false;
    }

    ~Bullet() {
        
    }

    void tick() override {
        pos.y += -direction.y * velocity * deltaTime;
        anim.getSprite().setPosition(pos);

        if (pos.y < 0) {
            markedForNegation = true;
        }
    }

    void draw(RenderWindow& window) override {
        window.draw(anim.getSprite());
    }

    bool markedForNegation;

private:
    float velocity;
    Vector2<float> direction;
    float power;
    Texture bt;
};

class Player: public Entity {
public:
    Player(Texture& t) {
        pos.y = height - t.getSize().y;

        anim.getSprite().setTexture(t);
        anim.getSprite().setScale(0.6f, 0.6f);

        pos.x = width / 4;

        tSize = t.getSize();
        scale = anim.getSprite().getScale();

        anim.getSprite().setPosition(pos);

        name = "Player";
    }

    void tick() override {
        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
            pos.x -= speed * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
            pos.x += speed * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
            pos.y -= speed * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
            pos.y += speed * deltaTime;

        pos.x = std::clamp<float>(pos.x, 0, width - (tSize.x * scale.x));
        pos.y = std::clamp<float>(pos.y, 0, height - (tSize.y * scale.y));

        anim.getSprite().setPosition(pos);
        fireTimer = fireTimer > -0.1f ? fireTimer - 0.1f : -0.1f;
    }

    void draw(RenderWindow& window) override {
        window.draw(anim.getSprite());
    }

    bool readyToFire() {
        return fireTimer < 0;
    }

    void resetFireTimer() {
        fireTimer = 20;
    }

private:
    int health = 100;
    float fireTimer = -0.1f;
    Vector2u tSize;
};

int main() {
    std::random_device* r = new std::random_device;
    std::mt19937 m(r->operator()());
    delete r;

    RenderWindow window(VideoMode(width, height), "Touhous Bizzare Adventure");

    std::list<Entity*> entities;

    Texture pt;
    pt.loadFromFile("images/player.png");

    Player* player = new Player(pt);
    player->setSpeed(500);

    entities.push_back(player);

    Clock clock;
    Video* touhou = new Video();

    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::F) && !touhou->isPlaying()) {
            touhou->install("images/touhou", 6572, 30.0003f, "audio/badapple.wav");
            entities.push_back(touhou);
            touhou->start();
        }

        if (Keyboard::isKeyPressed(Keyboard::Space) && player->readyToFire()) {
            entities.push_back(new Bullet(1, player->pos, 10));
            player->resetFireTimer();
        }

        for (auto it = entities.begin(); it != entities.end(); it++) {
            (*it)->tick();

            if ((*it)->name == "Bullet") {
                if (static_cast<Bullet*>(*it)->markedForNegation) {
                    auto e = (*it);
                    it = entities.erase(it);
                    delete e;
                }
            }
        }

        window.clear();

        for (auto e: entities)
            e->draw(window);

        window.display();
    }

    return 0;
}
