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

    Animation(const Animation& other) {
        sprite = other.sprite;
        fps = other.fps;
        frame = other.frame;
        frames = other.frames;
    }

    Animation(Texture& t, int x, int y, int w, int h, int count, float speed) {
        frame = 0;
        fps = speed;

        for (int i = 0; i < count; i++)
            frames.emplace_back(IntRect(x + i * w, y, w, h));

        sprite.setTexture(t);
        sprite.setOrigin(w / 2, h / 2);
        sprite.setTextureRect(frames[0]);
    }

    void update() {
        frame += fps;
        int n = frames.size();
        if (frame >= n) frame -= n;
        if (n > 0) sprite.setTextureRect(frames[static_cast<int>(frame)]);
    }

    Sprite& getSprite() {
        return sprite;
    }
 
private:
    Sprite sprite;
    std::vector<IntRect> frames;

    float frame = 0, fps = 0;
};

class Entity {
public:
    Entity() = default;

    Entity(const Entity& other) {
        name = other.name;
        pos = other.pos;
        scale = other.scale;
        anim = other.anim;
        speed = other.speed;
        hitRadius = other.hitRadius;
    }

    void config(Animation& a, Vector2f s, Vector2f p, float r) {
        anim = a;
        pos = p;
        hitRadius = r;
        scale = s;
        anim.getSprite().setScale(scale);
        anim.getSprite().setPosition(p);
    }

    virtual ~Entity() = default;

    virtual void tick() { }

    virtual void draw(RenderWindow& window) { }

    float getSpeed() const { return speed; }

    void setSpeed(const float x) { speed = x; }

    bool collision(const Entity& other) const {
        return (other.pos.x - pos.x) * (other.pos.x - pos.x) +
            (other.pos.y - pos.y) * (other.pos.y - pos.y) <
            (hitRadius + other.hitRadius) * (hitRadius + other.hitRadius);
    }

    std::string name;
    Vector2<float> pos;
    float hitRadius;
    Vector2f scale;

protected:
    Animation anim;
    float speed = 0;
};

class Video final : public Entity {
public:
    Video(){
        name = "Video";

        playing = false;
        accumulator = 0.0f;
        frame = 0;
        fps = 0;
    }

    // relies on files having format 'output-XXXX.png' naming format.
    void install(const std::string& frameFolder, const int numFrames, const float fps, const std::string& audioPath) {
        this->fps = 1 / fps;
        audio.openFromFile(audioPath);

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
    }

    void tick() override {
        if (!playing)
            return;

        accumulator += deltaTime;

        if (accumulator >= fps) {
            accumulator = 0;
            frame += 1;
            if (!(frame >= frames.size())) {
                sprite.setTexture(frames[frame]);
            } else {
                stop();
            }
        }
    }

    void start() {
        audio.play();
        playing = true;
        frame = 0;
        sprite.setTexture(frames[frame]);
    }

    void stop() {
        audio.stop();
        playing = false;
        frame = 0;
        accumulator = 0;
    }

    bool isPlaying() const {
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

class Enemy final: public Entity {
public:
    Enemy() {
        scale.x = 0.3f;
        scale.y = 0.3f;

        anim.getSprite().setScale(scale);
    }

    void tick() override {
	    
    }

    void draw(RenderWindow& window) override {
        window.draw(anim.getSprite());
    }
};

class Bullet final: public Entity {
public:
	/**
	 * \brief Bullet Constructor
	 * \param a Animation
	 * \param v velocity
	 * \param pos starting position
	 * \param p power
	 * \param ud true = up, false = down
	 */
	Bullet(float p, bool ud, float v): markedForNegation(false), upDown(ud), power(p), velocity(v) {
        scale.x = 0.5f;
        scale.y = 0.5f;
        name = "Bullet";
    }

    void tick() override {
        anim.update();
        pos.y -= upDown ? velocity * deltaTime : -velocity * deltaTime;
        anim.getSprite().setPosition(pos);

        if (pos.y < 0) {
            markedForNegation = true;
        }
    }

    void draw(RenderWindow& window) override {
        window.draw(anim.getSprite());
    }

    bool markedForNegation;
    bool upDown;

private:
    float velocity;
    float power;
};

class Player final: public Entity {
public:
    Player(Animation& a) {
        pos.y = height - a.getSprite().getTexture()->getSize().y;

        anim = a;
        anim.getSprite().setScale(0.6f, 0.6f);

        pos.x = width / 4;

        tSize = static_cast<Vector2f>(anim.getSprite().getTexture()->getSize()) * 0.6f;
        scale = anim.getSprite().getScale();

        anim.getSprite().setPosition(pos);

        name = "Player";
    }

    void tick() override {
        anim.update();

        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
            pos.x -= speed * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
            pos.x += speed * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
            pos.y -= speed * deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
            pos.y += speed * deltaTime;

        pos.x = std::clamp<float>(pos.x, tSize.x / 2, width - (tSize.x / 2));
        pos.y = std::clamp<float>(pos.y, tSize.y / 2, height - (tSize.y / 2));

        anim.getSprite().setPosition(pos);
        fireTimer = fireTimer > -0.1f ? fireTimer - 0.1f : -0.1f;
    }

    void draw(RenderWindow& window) override {
        window.draw(anim.getSprite());
    }

    bool readyToFire() const {
        return fireTimer < 0;
    }

    void resetFireTimer() {
        fireTimer = 2;
    }

private:
    int health = 100;
    float fireTimer = -0.1f;
    Vector2f tSize;
};

int main() {
    auto r = new std::random_device;
    std::mt19937 m(r->operator()());
    delete r;
    
    RenderWindow window(VideoMode(width, height), "Touhous Bizzare Adventure");

    std::list<Entity*> entities;

    Texture pt, bt, bt2, et;
    pt.loadFromFile("images/player.png");
    bt.loadFromFile("images/bullet.png");
    bt2.loadFromFile("images/bullet-2.png");
    et.loadFromFile("images/enemy.png");

    Animation ba, ba2, pa, ea;
    pa = Animation(pt, 0, 0, 336, 229, 1, 0.1f);
    ba = Animation(bt, 0, 0, 32, 64, 16, 0.5f);
    ba2 = Animation(bt2, 0, 0, 400, 400, 76, 0.5f);
    ea = Animation(et, 0, 0, 1100, 1120, 1, 0.1f);

    auto player = new Player(pa);
    auto enemy = new Enemy;
    enemy->config(ea, Vector2f(0.3f, 0.3f), Vector2f(width / 2, 0), 50);
    player->setSpeed(500);

    entities.push_back(player);
    entities.push_back(enemy);

    Clock clock;
    auto touhou = new Video;

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
            auto nb = new Bullet(10, true, 500);
            nb->config(ba, Vector2f(0.6f, 0.6f), player->pos, 10);
            entities.push_back(nb);
            player->resetFireTimer();
        }

        for (auto it = entities.begin(); it != entities.end(); it++) {
            (*it)->tick();

            if ((*it)->name == "Bullet") {
	            if (auto dcb = dynamic_cast<Bullet*>(*it); dcb != nullptr && dcb->markedForNegation) {
            		it = entities.erase(it);
            		delete dcb;
            	}
            }

            if (it == entities.end())
                break;
        }

        window.clear();

        for (auto e: entities)
            e->draw(window);

        window.display();
    }

    return 0;
}
