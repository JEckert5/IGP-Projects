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

static int width = 1000, height = 1000;

static float deltaTime = 0.0f;
static const Vector2f bulletScale(0.6f, 0.6f);
static constexpr float dioBulletVel = 200.f;
static constexpr float dioBulletDmg = 5;
static constexpr float playerBulletDmg = 10;
static constexpr float bulletRadius = 10;
static bool paused = false;

std::mt19937 m;

/**
 * \brief Naive interpolation.
 * \param a min
 * \param b max
 * \param t time
 * \return Interpolated value between a and b at t
 */
float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

/**
 * \brief Animation class for cycling through a sprite sheet or static png.
 */
class Animation {
public:
    Animation() = default;

    /**
     * \brief Copy Constructor.
     * \param other Animation to copy.
     */
    Animation(const Animation& other) {
        sprite = other.sprite;
        fps = other.fps;
        frame = other.frame;
        frames = other.frames;
    }

    /**
     * \brief Constructor that actually constructs
     * \param t Texture
     * \param x Origin x
     * \param y Origin y
     * \param w width of each sprite tile
     * \param h height of each sprite tile
     * \param count frame count
     * \param speed Time the animation takes(in seconds).
     */
    Animation(Texture& t, int x, int y, int w, int h, int count, float speed) {
        frame = 0;
        fps = speed;

        for (int i = 0; i < count; i++)
            frames.emplace_back(IntRect(x + i * w, y, w, h));

        sprite.setTexture(t);
        sprite.setOrigin(w / 2, h / 2);
        sprite.setTextureRect(frames[0]);
    }

    /**
     * \brief Update frame value and advance frame if near whole number.
     */
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

/**
 * \brief Base Entity class with no special functionality.
 */
class Entity {
public:
    Entity() = default;

    /**
     * \brief Copy constructor
     * \param other Enitity to copy.
     */
    Entity(const Entity& other) {
        name = other.name;
        pos = other.pos;
        scale = other.scale;
        anim = other.anim;
        speed = other.speed;
        hitRadius = other.hitRadius;
    }

    /**
     * \brief Configure various parameters of the Entity. Can be overloaded.
     * \param a Animation
     * \param s Scale
     * \param p Initial Position
     * \param r Hit circle radius
     */
    virtual void config(Animation& a, Vector2f s, Vector2f p, float r) {
        anim = a;
        pos = p;
        hitRadius = r;
        scale = s;
        anim.getSprite().setScale(scale);
        anim.getSprite().setPosition(p);
    }

    /**
     * \brief If an Entity needs to be instantiated without a base class, a lambda can be passed as the tick function.
     * \param t Lambda tick
     */
    virtual void setTick(std::function<void()> t) {
        optionalTick = std::move(t);
    }

    virtual ~Entity() = default;

    /**
     * \brief Default tick function. Can be overloaded.
     */
    virtual void tick() { 
        anim.update();
        if (optionalTick)
            optionalTick();
    }

    /**
     * \brief Default draw function.
     * \param window Reference to the main RenderWindow object
     */
    virtual void draw(RenderWindow& window) {
        window.draw(anim.getSprite());
    }

    /**
     * \brief Check for collision with another Entity.
     * \param other Entity to check against.
     * \return true if colliding with other object, false if not.
     */
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

/**
 * \brief Very stupid Video player class. DO NOT USE SERIOUSLY.
 */
class Video final : public Entity {
public:
	/**
	 * \brief Default constructor. Sets values.
	 */
	Video(){
        name = "Video";

        playing = false;
        accumulator = 0.0f;
        frame = 0;
        fps = 0;
    }

	/**
	 * \brief Load all necessary images. Images NEED to be in format 'output-XXXX.png'.
	 * \param frameFolder Path to folder containing images/frames
	 * \param numFrames number of frames
	 * \param fps the fps of the video
	 * \param audioPath path to the audio
	 */
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

	/**
	 * \brief Tick frames.
	 */
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

	/**
	 * \brief Start playback.
	 */
	void start() {
        audio.play();
        playing = true;
        frame = 0;
        sprite.setTexture(frames[frame]);
    }

	/**
	 * \brief Stop playback.
	 */
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

/**
 * \brief Player class. Implements moving around and shoot timing.
 */
class Player: public Entity {
public:
	/**
	 * \brief Default constructor. Takes an animation object.
	 * \param a Animation object reference
	 */
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

	/**
	 * \brief Tick position bounds + movement input.
	 */
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
        pos.y = std::clamp<float>(pos.y, tSize.y * 5.f, static_cast<float>(height) - (tSize.y * 1.5f));

        anim.getSprite().setPosition(pos);
        fireTimer = fireTimer > -0.1f ? fireTimer - deltaTime : fireTimer;
    }

    bool readyToFire() const {
        return fireTimer < 0;
    }

    virtual void resetFireTimer() {
        fireTimer = 0.12f;
    }

	/**
	 * \brief Applies damage to player.
	 * \param dmg damage
	 * \return health remaining.
	 */
	unsigned int damage(unsigned int dmg) {
        health -= dmg;

        if (health <= 0)
            markedForNegation = true;

        return health;
    }

	/**
	 * \brief reset all member values to default.
	 */
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
    enum class SchlerpType {
        INF = 1,
        LERP,
        NONE,
    };

    Bullet(Animation& a, Vector2f pos, float pow, float v, Entity* s, float radius): sType(SchlerpType::NONE), velocity(v), power(pow) {
        scale = bulletScale;
        anim = a;
        this->pos = pos;
        hitRadius = radius;
        markedForNegation = false;
        source = s;
        interpFunc = [](Vector2f v2, float t, float v, bool d) {
            return v2;
        };
    }

    void config(SchlerpType st, std::function<Vector2f(Vector2f, float, float, bool)> func) {
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


private:
    std::function<Vector2f(Vector2f, float, float, bool)> interpFunc;
    float schlerp = 0.001f;
    bool schlerpDir = true;
    SchlerpType sType;
    float velocity;
    float power;
    Entity* source;
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

            if (pulseCounter > pulseCounts[currentPattern - 1]){
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

    void resetFireTimer() override {
        fireTimer = 1.5f;
    }

    /**
     * \brief Three beams
     */
    void patternOne() {
        pulseTimer -= deltaTime;

        if (pulseTimer <= 0.f) {
            // auto db = new Bullet(dioBulletDmg, dioBulletVel, this);
            auto db = new Bullet(bulletAnim, pos, dioBulletDmg, dioBulletVel, this, bulletRadius);
            db->config(Bullet::SchlerpType::NONE, [](Vector2f v2, float t, float v, bool d) {
                v2.y += deltaTime * v;
                return v2;
            });
            help->push_back(db);

            db = nullptr;

            db = new Bullet(bulletAnim, pos, dioBulletDmg, dioBulletVel, this, bulletRadius);
            db->config(Bullet::SchlerpType::NONE, [](Vector2f v2, float t, float v, bool d) {
                v2.x += deltaTime * v;
                v2.y += deltaTime * v;

                return v2;
            });
            db->anim.getSprite().setRotation(135);
            help->push_back(db);

            db = nullptr;

            db = new Bullet(bulletAnim, pos, dioBulletDmg, dioBulletVel, this, bulletRadius);
            db->config(Bullet::SchlerpType::NONE, [](Vector2f v2, float t, float v, bool d) {
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
                center->pos.y += deltaTime * dioBulletVel;
            });

            help->push_back(center);


            for (int i = 0; i < 9; i++) {
                auto db = new Bullet(bulletAnim, pos, dioBulletDmg, 0, this, bulletRadius);
                db->config(Bullet::SchlerpType::INF, [center, i](Vector2f v2, float t, float v, bool d) {
                    const auto angle = static_cast<float>(i * 40) + t;
                    const float dist = t * 50;

                    v2.x = center->pos.x + dist * cosf(angle); //* deltaTime;
                    v2.y = center->pos.y + dist * sinf(angle); //* deltaTime;

                    return v2;
                });
                help->push_back(db);
                db = nullptr;
            }

            pulseTimer = 0.35f;
            pulseCounter += 1;
        }
    }

    /**
     * \brief Sin wave.
     */
    void patternThree() {
        pulseTimer -= deltaTime;

        if (pulseTimer <= 0.f) {
        	auto db = new Bullet(bulletAnim, pos, dioBulletDmg, dioBulletVel, this, bulletRadius);
            db->config(Bullet::SchlerpType::LERP, [](Vector2f v2, float t, float v, bool d) {
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
		    auto db = new Bullet(bulletAnim, pos, dioBulletDmg, dioBulletVel, this, bulletRadius);
	        db->config(Bullet::SchlerpType::LERP, [](Vector2f v2, float t, float v, bool d) { 
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
    Music music;
    music.openFromFile("audio/jojo.wav");

    Text dioHealth, playerHealth;
    Font font;
    dioHealth.setPosition(width * 0.7f, height * 0.75f);
    playerHealth.setPosition(width * 0.2f, height * 0.75f);

    if (!font.loadFromFile("images/MGS2.ttf")) {
        std::cout << "Could not load font" << std::endl;
        return -1;
    }

    dioHealth.setFont(font);
    playerHealth.setFont(font);
    dioHealth.setCharacterSize(50);
    playerHealth.setCharacterSize(50);
    
    RenderWindow window(VideoMode(width, height), "Touhous Bizzare Adventure");

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

    music.play();

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
                // auto nb = new Bullet(10, 850, player);
                auto nb = new Bullet(ba, player->pos, playerBulletDmg, 850, player, bulletRadius);
                nb->config(Bullet::SchlerpType::NONE, [](Vector2f v2, float t, float v, bool d) {
                    v2.y -= v * deltaTime;

                    return v2;
                });
                entities.push_back(nb);
                player->resetFireTimer();
            }

            for (auto it = entities.begin(); it != entities.end();) {
                (*it)->tick();

                if ((*it)->markedForNegation) {
                    it = entities.erase(it);
                }

                if (it != entities.end())
                    it++;
            }

            for (auto a : entities) {
                for (auto b : entities) {
                    a->collision(b);
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
                dioHealth.setPosition(width * 0.8f, height * 0.75f);
	        }
        }

        if (player->markedForNegation) {
            dioHealth.setString("Game Over!\nClick to retry");
            dioHealth.setPosition(width / 3, height / 2);
            paused = true;
        } else if (enemy->markedForNegation) {
            dioHealth.setString("You Won!\nClick to restart");
            dioHealth.setPosition(width / 3, height / 2);
            paused = true;
        } else {
            dioHealth.setString("Dio: " + std::to_string(enemy->health));
            playerHealth.setString("Health: " + std::to_string(player->health));
        }
        window.clear();

        for (auto e: entities)
            e->draw(window);

        window.draw(dioHealth);
        window.draw(playerHealth);

        window.display();
    }

    return 0;
}
