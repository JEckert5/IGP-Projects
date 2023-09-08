#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <random>
#include <vector>
#include <list>
#include <iostream>

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

    Entity(std::list<Entity*>* ents): globlEntities(ents) {}

    virtual void tick() { };

    virtual void draw(RenderWindow& window) { }

    float getSpeed() const { return speed; }

    const void setSpeed(float x) { speed = x; }

    std::string name;

protected:
    Animation anim;
    float speed;
    Vector2<float> pos;
    std::list<Entity*>* globlEntities = nullptr;
};

class Video: public Entity {
public:
    Video(std::list<Entity*>* ent){//: globlEntities(ent) {
        globlEntities = ent;
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
    Bullet(float v, Vector2<float> dir, float p, std::list<Entity*>* end): velocity(v), direction(dir), power(p) {
        globlEntities = end;
        bt.loadFromFile("images/bullet.gif");
        anim.getSprite().setTexture(bt);
        name = "Bullet";
        pos = dir;
    }

    ~Bullet() {
        // std::cout << "FORTNITE!!\n" << std::endl;
        globlEntities = nullptr;
    }

    void tick() override {
        pos.y += -direction.y * velocity * deltaTime;
        anim.getSprite().setPosition(pos);

        if (pos.y < 0) {
            // std::cout << "Hello.\n" << std::endl;
            markedForNegation = true;
            // std::cout << "Hellllsdfsdfsdf\n" << std::flush;
        }
    }

    void draw(RenderWindow& window) override {
        window.draw(anim.getSprite());
    }

    bool isMarkedForNegation() const {
        return markedForNegation;
    }

private:
    float velocity;
    Vector2<float> direction;
    float power;
    Texture bt;
    bool markedForNegation = false;
};

class Player: public Entity {
public:
    Player(Texture& t, std::list<Entity*>* ent) {
        globlEntities = ent;
        pos.y = height - t.getSize().y;

        anim.getSprite().setTexture(t);
        anim.getSprite().setScale(0.6f, 0.6f);

        pos.x = width / 4;

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

        // Instantiate new bullet
        if (Keyboard::isKeyPressed(Keyboard::Space) && fireTimer < 0) {
            globlEntities->push_back(new Bullet(1, Vector2<float>(pos), 10, globlEntities));
            fireTimer = 20;
            std::cout << "Space" << std::endl;
        }

        std::cout << "Gug" << std::endl;
        anim.getSprite().setPosition(pos);
        fireTimer = fireTimer > -0.1f ? fireTimer - 0.1f : -0.1f;
    }

    void draw(RenderWindow& window) override {
        window.draw(anim.getSprite());
    }

private:
    int health = 100;
    float fireTimer = -0.1f;
};

int main() {
    std::random_device* r = new std::random_device;
    std::mt19937 m(r->operator()());
    delete r;

    RenderWindow window(VideoMode(width, height), "Touhous Bizzare Adventure");

    std::list<Entity*> entities;

    Texture pt;
    pt.loadFromFile("images/player.png");

    Player* player = new Player(pt, &entities);
    player->setSpeed(500);

    entities.push_back(player);

    Clock clock;
    Video* touhou = new Video(&entities);

    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();

            if (Keyboard::isKeyPressed(Keyboard::F) && !touhou->isPlaying()) {
                touhou->install("images/touhou", 6572, 30.0003f, "audio/badapple.wav");
                entities.push_back(touhou);
                touhou->start();
            }
        }

        for (auto it = entities.begin(); it != entities.end(); it++) {
            (*it)->tick();

            if ((*it)->name == "Bullet") {
                if (static_cast<Bullet*>(*it)->isMarkedForNegation()) {
                    auto e = (*it);
                    it = entities.erase(it);
                    delete e;
                }
            }
        }

        std::cout << "Help" << std::endl;

        window.clear();

        for (auto e: entities)
            e->draw(window);

        window.display();
    }

    return 0;
}
