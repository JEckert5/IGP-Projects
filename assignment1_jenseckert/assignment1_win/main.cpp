#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <random>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace sf;

int width = 1000, height = 1000;

static float deltaTime = 0.0f;
static const Vector2f bulletScale(0.6f, 0.6f);
static constexpr float dioBulletVel = 100.f;
static constexpr float dioBulletDmg = 5;
static bool paused = false;

std::mt19937 m;

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

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

    virtual void config(Animation& a, Vector2f s, Vector2f p, float r) {
        anim = a;
        pos = p;
        hitRadius = r;
        scale = s;
        anim.getSprite().setScale(scale);
        anim.getSprite().setPosition(p);
    }

    virtual void setTick(std::function<void()> t) {
        optionalTick = t;
    }

    virtual ~Entity() = default;

    virtual void tick() { 
        anim.update();
        if (optionalTick)
            optionalTick();
    }

    virtual void draw(RenderWindow& window) {
        window.draw(anim.getSprite());
    }

    virtual bool collision(Entity* other) {
        if (other == this)
            return false;

        return (other->pos.x - pos.x) * (other->pos.x - pos.x) +
            (other->pos.y - pos.y) * (other->pos.y - pos.y) <
            (hitRadius + other->hitRadius) * (hitRadius + other->hitRadius);
    }

    std::string name;
    Vector2<float> pos;
    float hitRadius;
    Vector2f scale;
    bool markedForNegation = false;
    Animation anim;
    float speed = 0;
    std::function<void()> optionalTick;
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

class Player: public Entity {
public:
    Player(Animation& a) {
        pos.y = height - a.getSprite().getTexture()->getSize().y;

        scale = Vector2f(0.4f, 0.4f);

        anim = a;
        anim.getSprite().setScale(scale);

        pos.x = width / 4;

        tSize = static_cast<Vector2f>(anim.getSprite().getTexture()->getSize()) * scale.x;

        anim.getSprite().setPosition(pos);

        name = "Player";
        health = 100;
        hitRadius = 50;
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

        pos.x = std::clamp<float>(pos.x, tSize.x *1.5f, static_cast<float>(width) - (tSize.x * 1.5f));
        pos.y = std::clamp<float>(pos.y, tSize.y * 1.5f, static_cast<float>(height) - (tSize.y * 1.5f));

        anim.getSprite().setPosition(pos);
        fireTimer = fireTimer > -0.1f ? fireTimer - deltaTime : fireTimer;
    }

    void draw(RenderWindow& window) override {
        window.draw(anim.getSprite());

        CircleShape circle(hitRadius);
        circle.setFillColor(Color(255, 0, 0, 170));
        circle.setPosition(pos);
        circle.setOrigin(hitRadius, hitRadius);

        // window.draw(circle);
    }

    bool readyToFire() const {
        return fireTimer < 0;
    }

    virtual void resetFireTimer() {
        fireTimer = 0.12f;
    }

    unsigned int damage(unsigned int dmg) {
        health -= dmg;

        if (health <= 0)
            markedForNegation = true;

        return health;
    }

    virtual void reset() {
        pos.y = height - tSize.y;
        pos.x = width / 4;
        markedForNegation = false;
        resetFireTimer();
        health = 100;
    }

    int health;
    Vector2f tSize;

protected:
    float fireTimer = -0.1f;
};

class Bullet: public Entity {
public:
	Bullet(float p, float v, Entity* s): velocity(v), power(p) {
        scale.x = 0.5f;
        scale.y = 0.5f;
        name = "Bullet";
        source = s;
        markedForNegation = false;
    }

    void tick() override {
        anim.update();
        pos.y -= velocity * deltaTime;
        anim.getSprite().setPosition(pos);
        // anim.getSprite().setRotation(upDown ? 0 : 180);

        if (pos.y < 0 || pos.y > height || pos.x < 0 || pos.x > width)
            markedForNegation = true;
    }

    void draw(RenderWindow& window) override {
        window.draw(anim.getSprite());

        // CircleShape circle(hitRadius);
        // circle.setFillColor(Color(255, 0, 0, 170));
        // circle.setPosition(pos);
        // circle.setOrigin(hitRadius, hitRadius);

        // window.draw(circle);
    }

    bool collision(Entity* other) override {
        if (other == source)
            return false;

        if (Entity::collision(other)) {
            // std::cout << "Collision in bullet" << std::endl;
            if (other->name == "Dio" || other->name == "Player") {
                dynamic_cast<Player*>(other)->damage(power);
                markedForNegation = true;
            }

            return true;
        }

        return false;
	}

protected:
    float velocity;
    float power;
    Entity* source;
};

class DioBullet: public Bullet {
public:
    DioBullet(float p, float v, Entity* s): Bullet(p, v, s) { }

    enum class SchlerpType {
	    INF = 1,
        LERP,
        NONE,
    };

    void config(Animation& a, Vector2f s, Vector2f p, float r, SchlerpType st, std::function<Vector2f(Vector2f, float, float, bool)> func) {
        Bullet::config(a, s, p, r);

        interpFunc = std::move(func);
        sType = st;
    }

    void tick() {
        pos = interpFunc(pos, schlerp, velocity, schlerpDir);

        switch (sType) {
        case SchlerpType::INF:
            schlerp += deltaTime;
        	break;
        case SchlerpType::LERP:
            schlerp = schlerpDir ? schlerp + deltaTime : schlerp - deltaTime;
            schlerp = std::clamp<float>(schlerp, 0.f, 1.f);

            if (0.f == schlerp)
                schlerpDir = true;
            else if (schlerp == 1.f)
                schlerpDir = false;

            break;
        case SchlerpType::NONE:
            break;
        }

        anim.update();
        anim.getSprite().setPosition(pos);

        if (pos.y < 0 || pos.y > height || pos.x < 0 || pos.x > width)
            markedForNegation = true;
    }
private:
    std::function<Vector2f(Vector2f, float, float, bool)> interpFunc;
    // Vector2f (*interpFunc)(Vector2f x, float y, float v, bool sd);
    float schlerp = 0.001f;
    bool schlerpDir = true;
    SchlerpType sType;
};

class Dio final: public Player {
public:
    Dio(Animation& a, const int& h, std::list<Entity*>* ptr): Player(a), help(ptr) {
        scale.x = 0.3f;
        scale.y = 0.3f;

        anim.getSprite().setScale(scale);
        name = "Dio";
        health = h;
    }

    ~Dio() override {
        help = nullptr;
        delete help;
    }

    void tick() override {
        if (readyToFire() && !firing) {
            resetFireTimer();
            firing = true;
            currentPattern = m() % 4 + 1;
        } else if (firing) {
            switch (currentPattern) {
            case 1:
                patternOne();
                break;
            case 2:
                patternTwo();
                break;
            case 3:
                patternThree();
                break;
            case 4:
                patternFour();
                break;
            default:
                break;
            }

            // patternFour();

            if (pulseCounter >= pulseCounts[currentPattern - 1]){
                pulseCounter = 0;
                firing = false;
            }
        }

        if (moveTimer <= 0) {
            moveTimer = m() % 4 + 1;

            if (m() % 500 > 250) {
                moveDir = 1;
            } else {
                moveDir = -1;
            }
        } else
            moveTimer -= deltaTime;

        if (pos.x <= tSize.x / 2) {
            moveTimer = m() % 4 + 1;

            moveDir = 1;
        } else if (pos.x >= width - tSize.x / 2) {
            moveTimer = m() % 4 + 1;

            moveDir = -1;
        }

        pos.x += moveDir * speed * deltaTime;

        anim.getSprite().setPosition(pos);
        fireTimer = fireTimer > -0.1f ? fireTimer - deltaTime : fireTimer;
    }

    void config(Animation& a, const Animation& bAnim, Vector2f s, Vector2f p, float r) {
        Entity::config(a, s, p, r);

        tSize = static_cast<Vector2f>(anim.getSprite().getTexture()->getSize()) * scale.x;
        bulletAnim = bAnim;
    }

    void draw(RenderWindow& window) override {
        CircleShape circle(hitRadius);
        circle.setFillColor(Color(255, 0, 0, 170));
        circle.setPosition(pos);
        circle.setOrigin(hitRadius, hitRadius);

        window.draw(anim.getSprite());
        // window.draw(circle);
    }

    void resetFireTimer() override {
        fireTimer = 2.5f;
    }

    /**
     * \brief Three beams
     */
    void patternOne() {
        pulseTimer -= deltaTime;

        if (pulseTimer <= 0.f) {
            auto db = new DioBullet(dioBulletDmg, dioBulletVel, this);
            db->config(bulletAnim, bulletScale, pos, 10, DioBullet::SchlerpType::NONE, [](Vector2f v2, float t, float v, bool d) { 
                v2.x += 0;
                v2.y += deltaTime * v;
                return v2;
            });
            help->push_back(db);

            db = nullptr;

            db = new DioBullet(dioBulletDmg, dioBulletVel, this);

            //auto db2 = new DioBullet(15, 50, this);
            db->config(bulletAnim, bulletScale, pos, 10, DioBullet::SchlerpType::NONE, [](Vector2f v2, float t, float v, bool d) {
                v2.x += deltaTime * v;
                v2.y += deltaTime * v;

                return v2;
            });
            db->anim.getSprite().setRotation(135);
            help->push_back(db);

            db = nullptr;

            db = new DioBullet(dioBulletDmg, dioBulletVel, this);
            db->config(bulletAnim, bulletScale, pos, 10, DioBullet::SchlerpType::NONE, [](Vector2f v2, float t, float v, bool d) {
                v2.x -= deltaTime * v;
                v2.y += deltaTime * v;

                return v2;
            });
            db->anim.getSprite().setRotation(225);
            help->push_back(db);

            db = nullptr;
            delete db;

            pulseTimer = 0.05f;
            pulseCounter += 1;
        } 
    }

    /**
     * \brief Creates a ring of bullets that expand outward while rotating.
     */
    void patternTwo() {
        pulseTimer -= deltaTime;

        if (pulseTimer <= 0.f) {
            auto center = new Entity;
            center->pos = pos;

            center->setTick([center]() {
                center->pos.y += deltaTime * 100;
            });

            help->push_back(center);


            for (int i = 0; i < 9; i++) {
                auto db = new DioBullet(dioBulletDmg, dioBulletVel, this);
                db->config(bulletAnim, bulletScale, center->pos, 10, DioBullet::SchlerpType::INF, [center, i](Vector2f v2, float t, float v, bool d) {
                    const auto angle = static_cast<float>(i * 40) + t;
                    const float dist = t * 50;

                    v2.x = center->pos.x + dist * cosf(angle); //* deltaTime;
                    v2.y = center->pos.y + dist * sinf(angle); //* deltaTime;

                    return v2;
                });
                help->push_back(db);
                db = nullptr;
            }
            

            pulseTimer = 0.5f;
            pulseCounter += 1;
        }
    }

    /**
     * \brief Sin wave.
     */
    void patternThree() {
        pulseTimer -= deltaTime;

        if (pulseTimer <= 0.f) {
        	auto db = new DioBullet(dioBulletDmg, dioBulletVel, this);
            db->config(bulletAnim, bulletScale, pos, 10,DioBullet::SchlerpType::LERP, [](Vector2f v2, float t, float v, bool d) {
                v2.x += (d ? lerp(0, 200, t) : -lerp(0, 200, t)) * deltaTime;
                v2.y += v * deltaTime;

                return v2;
            });
            help->push_back(db);

            pulseTimer = 0.1f;
            pulseCounter += 1;
        }
    }

    /**
     * \brief Rain
     */
    void patternFour() {
	    // << "Woah4\n" << std::flush;
        pulseTimer -= deltaTime;

        if (pulseTimer <= 0.f) {
		    auto db = new DioBullet(dioBulletDmg, dioBulletVel, this);
	        db->config(bulletAnim, bulletScale, pos, 10, DioBullet::SchlerpType::LERP, [](Vector2f v2, float t, float v, bool d) { 
	            v2.x += 0;
                v2.y += (d ? lerp(0, 300, t) : -lerp(0, 100, t)) * deltaTime;
	            return v2;

	        });
	        help->push_back(db);

            pulseTimer = 0.185f;
            pulseCounter += 1;
        }
    }

    void reset() override {
        pos = Vector2f(width / 2, height / 4);
        markedForNegation = false;
        health = 1000;
        resetFireTimer();
        pulseCounter = 0;
        currentPattern = 0;
        firing = false;
    }

private:
    float moveTimer = 0;
    // 1 = right, -1 = left;
    int moveDir = 1;
    std::list<Entity*>* help;
    Animation bulletAnim;
    bool firing = false;
    float pulseTimer = 0.f;
    int pulseCounts[4] = {5, 1, 6, 3};
    int pulseCounter = 0;
    int currentPattern = 0;
};

int main() {
    auto r = new std::random_device;
    m.seed((*r)());
    delete r;

    Text text;
    Font font;
    text.setPosition(width * 0.8f, height * 0.75f);

    if (!font.loadFromFile("images/MGS2.ttf")) {
        std::cout << "Could not load font" << std::endl;
        return -1;
    }

    text.setFont(font);
    text.setCharacterSize(50);
    
    RenderWindow window(VideoMode(width, height), "Touhous Bizzare Adventure");
    // window.setFramerateLimit(60);

    std::list<Entity*> entities;

    Texture pt, bt, bt2, et, bgr;
    pt.loadFromFile("images/player.png");
    bt.loadFromFile("images/bullet.png");
    bt2.loadFromFile("images/bullet-2.png");
    et.loadFromFile("images/enemy.png");
    bgr.loadFromFile("images/optimum prime.png");

    Animation ba, ba2, pa, ea, bgra;
    bgra = Animation(bgr, 0, 0, 400, 399, 1, 0.1f);
    pa = Animation(pt, 0, 0, 336, 229, 1, 0.1f);
    ba = Animation(bt, 0, 0, 32, 64, 16, 0.5f);
    ba2 = Animation(bt2, 0, 0, 32, 64, 16, 0.5f);
    ea = Animation(et, 0, 0, 1100, 1120, 1, 0.1f);

    ba2.getSprite().setRotation(180);

    auto player = new Player(pa);
    auto enemy = new Dio(ea, 1000, &entities);
    auto bgrnd = new Entity;
    bgrnd->config(bgra, Vector2f(2.f, 2.f), Vector2f(width / 2, height / 2), 0);
    enemy->config(ea, ba2, Vector2f(0.3f, 0.3f), Vector2f(width / 2, height / 4), 100);
    player->speed = 500;
    enemy->speed = 125;

    entities.push_back(bgrnd);
    entities.push_back(player);
    entities.push_back(enemy);

    Clock clock;
    auto touhou = new Video;

    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
        }

        if (!paused) {
            if (Keyboard::isKeyPressed(Keyboard::F) && !touhou->isPlaying()) {
                touhou->install("images/touhou", 6572, 30.0003f, "audio/badapple.wav");
                entities.push_back(touhou);
                touhou->start();
            }

            if (Keyboard::isKeyPressed(Keyboard::Space) && player->readyToFire() && !player->markedForNegation) {
                auto nb = new Bullet(10, 850, player);
                nb->config(ba, Vector2f(0.6f, 0.6f), player->pos, 10);
                entities.push_back(nb);
                player->resetFireTimer();
            }

            for (auto it = entities.begin(); it != entities.end();) {
                (*it)->tick();

                if ((*it)->markedForNegation) {
                    // delete* it;
                    it = entities.erase(it);
                }

                if (it != entities.end())
                    it++;
            }

            for (auto e : entities) {
                for (auto b : entities) {
                    e->collision(b);
                }
            }
        } else { // is paused
	        if (Mouse::isButtonPressed(Mouse::Left)) { // restart
                paused = false;

                player->reset();
                enemy->reset();
                entities.clear();
                entities.push_back(bgrnd);
                entities.push_back(player);
                entities.push_back(enemy);
                text.setPosition(width * 0.8f, height * 0.75f);
	        }
        }

        if (player->markedForNegation) {
            text.setString("Game Over!\nClick to retry");
            text.setPosition(width / 3, height / 2);
            paused = true;
        } else if (enemy->markedForNegation) {
            text.setString("You Won!\nClick to restart");
            text.setPosition(width / 3, height / 2);
            paused = true;
        }
        else
            text.setString("Dio: " + std::to_string(enemy->health));

        window.clear();

        for (auto e: entities)
            e->draw(window);

        window.draw(text);

        window.display();
    }

    return 0;
}
