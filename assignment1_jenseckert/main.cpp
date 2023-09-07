#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <random>
#include <vector>
#include <list>
#include <iostream>

using namespace sf;

int width = 800, height = 600;

static float deltaTime = 0.0f;

// Ripped from the asteriod demo.
class Animation {
public:
    Animation() { }

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

private:
    Sprite sprite;
    std::vector<IntRect> frames;

    float frame, fps;
};

class Video {
public:
    // relies on files having format 'output-XXXX.png' naming format.
    Video(const std::string& frameFolder, int numFrames, float fps, const std::string& audioPath) { 
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

    void update() {
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

    void draw(RenderWindow& window) {
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

class Entity {
public:
    Entity() { }

    virtual void tick() {};

    virtual void draw(RenderWindow& window) {
        
    }

    float getSpeed() const {
        return speed;
    }

    const void setSpeed(float x) {
        speed = x;
    }

protected:
    Animation anim;
    float speed;
};

class Enemy: public Entity {
    void draw(RenderWindow& window) override {

    }
};

class Player: public Entity {
public:
    Player(Texture& t) {
        y = height - t.getSize().y;

        sprite.setTexture(t);

        x = width / 4;

        sprite.setPosition(x, y);
    }

    void tick() override {
        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
            x -= speed * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
            x += speed * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
            y -= speed * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
            y += speed * deltaTime;

        sprite.setPosition(x, y);
    }

    void draw(RenderWindow& window) override {
        window.draw(sprite);
    }

    Sprite sprite;

private:
    float x, y;
    int health = 100;
};

int main() {
    std::random_device* r = new std::random_device;
    std::mt19937 m(r->operator()());
    delete r;

    RenderWindow window(VideoMode(width, height), "Touhous Bizzare Adventure");

    std::list<Entity*> entities;

    Texture pt, et, bt;
    pt.loadFromFile("images/player.png");

    Player* player = new Player(pt);
    player->setSpeed(500);

    entities.push_back(player);

    Clock clock;

    Video touhou("images/touhou", 6572, 30.0003f, "audio/badapple.wav");

    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (Keyboard::isKeyPressed(Keyboard::F)) {
                touhou.start();
            }
        }

        for (auto& e: entities) {
            e->tick();
        }

        window.clear();
        
        for (auto& e: entities) {
            e->draw(window);
        }

        if (touhou.isPlaying()) {
            touhou.update();
            touhou.draw(window);
        }

        window.display();
    }

    return 0;
}