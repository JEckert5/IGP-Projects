#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <random>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

using namespace sf;

int width = 1280, height = 1280;

static float deltaTime = 0.0f;

std::mt19937 m;

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

    virtual ~Entity() = default;

    virtual void tick() { }

    virtual void draw(RenderWindow& window) { }

    float getSpeed() const { return speed; }

    void setSpeed(const float x) { speed = x; }

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

class Player final : public Entity {
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
        fireTimer = fireTimer > -0.1f ? fireTimer - 0.1f : fireTimer;
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

    unsigned int damage(unsigned int dmg) {
        health -= dmg;

        if (health <= 0)
            markedForNegation = true;

        return health;
    }

private:
    int health = 100;
    float fireTimer = -0.1f;
    Vector2f tSize;
};

class Dio final: public Entity {
public:
    Dio(const Uint32& h, std::list<Entity*>* ptr): health(h), help(ptr) {
        scale.x = 0.3f;
        scale.y = 0.3f;

        anim.getSprite().setScale(scale);
        name = "Dio";
    }

    void tick() override {
        if (m() % 100 > 25 && readyToFire()) {
            resetFireTimer();

            switch (m() % 5 + 1) {
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
                std::cout << "How" << std::endl;
                break;
            }
        }


        if (moveTimer <= 0) {
            moveTimer = m() % 4 + 1;

            if (m() % 500 > 250) {
                moveDir = 1;
            } else {
                moveDir = -1;
            }
        } else if (pos.x <= tSize.x / 2) {
            moveTimer = m() % 4 + 1;

            moveDir = 1;
        } else if (pos.x >= width - tSize.x / 2) {
            moveTimer = m() % 4 + 1;

            moveDir = -1;
        } else {
            moveTimer -= deltaTime * 1.25f;
        }

        std::cout << moveTimer << std::endl;

        pos.x += moveDir * speed * deltaTime;

        anim.getSprite().setPosition(pos);
        fireTimer = fireTimer > -0.1f ? fireTimer - 0.1f : fireTimer;
    }

    void config(Animation& a, Vector2f s, Vector2f p, float r) override {
        Entity::config(a, s, p, r);

        tSize = static_cast<Vector2f>(anim.getSprite().getTexture()->getSize()) * 0.3f;
    }

    void draw(RenderWindow& window) override {
        CircleShape circle(hitRadius);
        circle.setFillColor(Color(255, 0, 0, 170));
        circle.setPosition(pos);
        circle.setOrigin(hitRadius, hitRadius);

        window.draw(anim.getSprite());
        // window.draw(circle);
    }

    unsigned int damage(unsigned int dmg) {
        health -= dmg;

        if (health <= 0)
            markedForNegation = true;

        return health;
    }

    bool readyToFire() const {
        return fireTimer < 0;
    }

    int getHealth() const {
        return health;
    }

    void resetFireTimer() {
        fireTimer = 2;
    }

    // Fuck
    void patternOne() {
	    
    }

    void patternTwo() {
	    
    }

    void patternThree() {
	    
    }

    void patternFour() {
	    
    }

private:
    unsigned int health;
    float fireTimer = -0.1f;
    float moveTimer = 0;
    // 1 = right, -1 = left;
    int moveDir = 1;
    Vector2f tSize;
    std::list<Entity*>* help;
};

class Bullet: public Entity {
public:
	/**
	 * \brief Bullet Constructor
	 * \param v velocity
	 * \param pos starting position
	 * \param p power
	 * \param ud true = up, false = down
	 * \param s the source of the bullet
	 */
	Bullet(float p, bool ud, float v, Entity* s): upDown(ud), velocity(v), power(p) {
        scale.x = 0.5f;
        scale.y = 0.5f;
        name = "Bullet";
        source = s;
        markedForNegation = false;
    }

    void tick() override {
        anim.update();
        pos.y -= upDown ? velocity * deltaTime : -velocity * deltaTime;
        anim.getSprite().setPosition(pos);
        anim.getSprite().setRotation(upDown ? 0 : 180);

        if (pos.y < 0 || pos.y > height || pos.x < 0 || pos.x > width)
            markedForNegation = true;
    }

    void draw(RenderWindow& window) override {
        window.draw(anim.getSprite());

        CircleShape circle(hitRadius);
        circle.setFillColor(Color(255, 0, 0, 170));
        circle.setPosition(pos);
        circle.setOrigin(hitRadius, hitRadius);

        // window.draw(circle);
    }

    bool collision(Entity* other) override {
        if (other == source)
            return false;

        if (Entity::collision(other)) {
            std::cout << "Collision in bullet" << std::endl;
            if (other->name == "Dio") {
                dynamic_cast<Dio*>(other)->damage(power);
                markedForNegation = true;
            } else if (other->name == "Player") {
                dynamic_cast<Player*>(other)->damage(power);
                markedForNegation = true;
            }

            return true;
        }

        return false;
	}

    bool upDown;

protected:
    float velocity;
    float power;
    Entity* source;
};

class DioBullet: public Bullet {
public:
    void config(Animation& a, Vector2f s, Vector2f p, float r, Vector2f dir) {
        direction = dir;
    }

    void tick() {
	    
    }
private:
    Vector2f direction;
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
    text.setCharacterSize(100);
    
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
    ba2 = Animation(bt2, 0, 0, 32, 64, 16, 0.5f);
    ea = Animation(et, 0, 0, 1100, 1120, 1, 0.1f);

    auto player = new Player(pa);
    auto enemy = new Dio(5000, &entities);
    enemy->config(ea, Vector2f(0.3f, 0.3f), Vector2f(width / 2, height / 4), 100);
    player->setSpeed(500);
    enemy->setSpeed(200);

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

        if (Keyboard::isKeyPressed(Keyboard::F) && !touhou->isPlaying()) {
            touhou->install("images/touhou", 6572, 30.0003f, "audio/badapple.wav");
            entities.push_back(touhou);
            touhou->start();
        }

        if (Keyboard::isKeyPressed(Keyboard::Space) && player->readyToFire()) {
            auto nb = new Bullet(10, true, 750, player);
            nb->config(ba, Vector2f(0.6f, 0.6f), player->pos, player->pos, 10);
            entities.push_back(nb);
            player->resetFireTimer();
        }

        for (auto it = entities.begin(); it != entities.end();) {
            (*it)->tick();

            if ((*it)->markedForNegation)
                it = entities.erase(it);

            if (it != entities.end())
                it++;
        }

        for (auto e: entities) {
	        for (auto b: entities) {
                e->collision(b);
	        }
        }

        text.setString("Dio: " + std::to_string(enemy->getHealth()));

        window.clear();

        for (auto e: entities)
            e->draw(window);

        window.draw(text);

        window.display();
    }

    return 0;
}
