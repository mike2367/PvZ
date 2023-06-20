#ifndef GAMEOBJECT_HPP__
#define GAMEOBJECT_HPP__

#include <memory>
#include <list>
#include "./Framework/ObjectBase.hpp"

enum class Holding
{
    ISEMPTY,
    ISSUNFLOWER,
    ISPEASHOOTER,
    ISWALLNUT,
    ISCHERRY_BOMB,
    ISREPEATER,
    ISSHOVEL
};

enum class Type
{
    SUN,
    PLANTINGSPOT,
    ZOMBIE,
    PEA,
    EXPLOSION,
    BACKGROUND,
    SEED,
    COOLNG_MASK,
    PLANT,
    NONE
};
enum class Hits
{
    EXPLOSION,
    PLANTS,
    PEA,
    ZOMBIE,
    NONE
};
enum class Plants {
    SUNFLOWER,
    PEASHOOTER,
    WALLNUT,
    CHERRY_BOMB,
    REPEATER,
    NONE
};

// Declares the class name GameWorld so that its pointers can be used.
class GameWorld;
using pGameWorld = std::shared_ptr<GameWorld>;

class GameObject : public ObjectBase, public std::enable_shared_from_this<GameObject> {
public:
    // Use shared_from_this() instead of "this".
    using std::enable_shared_from_this<GameObject>::shared_from_this;
    GameObject(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :ObjectBase(imageID, x, y, layer, width, height, animID), Hp(0), type(Type::NONE), which(Plants::NONE), p_this(world_this) {
    };
    pGameWorld p_this;
    Type check_type() const;
    void change_type(Type ty);
    int check_hp() const;
    void change_hp(int hp);
    Hits check_hit() const;
    void change_hit(Hits type);
    bool check_walking() const;
    void change_walking(bool walking);
private:
    bool iswalking = true;
    Hits hit_type = Hits::NONE;
    int Hp;
    Type type;
    Plants which;

};
class Background :public GameObject
{
public:
    Background(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :GameObject(imageID, x, y, layer, width, height, animID, world_this) {
        change_type(Type::BACKGROUND);
        change_hp(500);
    };
    virtual void Update() override;
    virtual void OnClick() override;
};

class PlantingSpot :public GameObject
{
public:
    PlantingSpot(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :GameObject(imageID, x, y, layer, width, height, animID, world_this) {
        change_type(Type::PLANTINGSPOT);
        change_hp(500);
    };
    virtual void Update() override;
    virtual void OnClick() override;

};
class Sun :public GameObject
{
public:
    Sun(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, bool Isnatural, pGameWorld world_this) :GameObject(imageID, x, y, layer, width, height, animID, world_this), isnatural(Isnatural) {
        change_type(Type::SUN);
        change_hp(500);
        m_speed_v = 4;
        move_timer = 12;
        disappear_timer = 300;
        down_timer = randInt(63, 263);
    };
    bool check_source() const;
    virtual void Update() override;
    virtual void OnClick() override;
private:
    bool isnatural;
    int m_speed_v;
    int move_timer;
    int disappear_timer;
    int down_timer;
};

class Seed :public GameObject
{
public:
    Seed(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, int cost, Holding seed, int time, pGameWorld world_this) :GameObject(imageID, x, y, layer, width, height, animID, world_this), Cost(cost), timer(time) {
        change_type(Type::SEED);
        whichSeed = seed;
        change_hp(500);
    };
    virtual void Update() override;
    virtual void OnClick() override;

private:
    const int timer;
    Holding whichSeed;
    const int Cost;

};

class Sunflower_Seed :public Seed
{
public:
    Sunflower_Seed(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, int cost, Holding seed, int time, pGameWorld world_this) :Seed(imageID, x, y, layer, width, height, animID, cost, seed, time, world_this) {
    };

};

class Wallnut_Seed :public Seed
{
public:
    Wallnut_Seed(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, int cost, Holding seed, int time, pGameWorld world_this) :Seed(imageID, x, y, layer, width, height, animID, cost, seed, time, world_this) {
    };

};
class Peashooter_Seed :public Seed
{
public:
    Peashooter_Seed(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, int cost, Holding seed, int time, pGameWorld world_this) :Seed(imageID, x, y, layer, width, height, animID, cost, seed, time, world_this) {
    };

};
class Cherry_bomb_Seed :public Seed
{
public:
    Cherry_bomb_Seed(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, int cost, Holding seed, int time, pGameWorld world_this) :Seed(imageID, x, y, layer, width, height, animID, cost, seed, time, world_this) {
    };

};
class Repeater_seed :public Seed
{
public:
    Repeater_seed(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, int cost, Holding seed, int time, pGameWorld world_this) :Seed(imageID, x, y, layer, width, height, animID, cost, seed, time, world_this) {
    };
};
class Cooling_mask :public GameObject
{
public:
    Cooling_mask(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, int time, pGameWorld world_this) :GameObject(imageID, x, y, layer, width, height, animID, world_this), timer(time) {
        change_hp(500);
    };
    virtual void Update() override;
    virtual void OnClick() override;
private:
    int timer;


};
class Plant :public GameObject
{
public:
    Plant(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :GameObject(imageID, x, y, layer, width, height, animID, world_this) {
        change_type(Type::PLANT);
        change_hp(500);
    };
    virtual void OnClick() override;

};

class Sunflower :public Plant
{
public:
    Sunflower(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :Plant(imageID, x, y, layer, width, height, animID, world_this) {
        get_first_sun_time();
        change_hp(300);
    };
    virtual void Update() override;
    void get_first_sun_time();
private:
    int first_sun_time;
    bool generate_first = false;
    int sun_timer = 600;
};
class Wallnut :public Plant
{
public:
    Wallnut(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :Plant(imageID, x, y, layer, width, height, animID, world_this)
    {
        change_hp(4000);
    };
    virtual void Update()override;
};
class Cherry_Bomb :public Plant
{
public:
    Cherry_Bomb(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :Plant(imageID, x, y, layer, width, height, animID, world_this)
    {
        change_hp(4000);
    };
    virtual void Update()override;
private:
    int boom_timer = 15;

};
class Explosion :public GameObject
{
public:
    Explosion(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :GameObject(imageID, x, y, layer, width, height, animID, world_this)
    {
        change_type(Type::EXPLOSION);
        change_hp(500);
    };
    virtual void Update()override;
    virtual void OnClick() override;
private:
    int timer = 3;
};

class Peashooter :public Plant
{
public:
    Peashooter(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :Plant(imageID, x, y, layer, width, height, animID, world_this)
    {
        change_hp(300);
    };
    virtual void Update()override;
private:
    int shoot_timer = 0;

};
class Pea :public GameObject
{
public:
    Pea(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :GameObject(imageID, x, y, layer, width, height, animID, world_this)
    {
        change_type(Type::PEA);
        change_hp(500);
    };
    virtual void Update()override;
    virtual void OnClick() override;

};
class Repeater :public Plant
{
public:
    Repeater(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :Plant(imageID, x, y, layer, width, height, animID, world_this)
    {
        change_hp(300);
    };
    virtual void Update()override;
private:
    bool second_start = false;
    int shoot_timer = 0;
    int second_timer = 5;
};
class Shovel :public GameObject
{
public:
    Shovel(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :GameObject(imageID, x, y, layer, width, height, animID, world_this)
    {
        change_hp(500);
    };
    virtual void Update()override;
    virtual void OnClick() override;
};
class Zombie : public GameObject
{
public:
    Zombie(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :GameObject(imageID, x, y, layer, width, height, animID, world_this)
    {
        change_type(Type::ZOMBIE);
        change_hp(200);
    };
    virtual void Update()override;
    virtual void OnClick() override;
private:


};
class Pole_zombie : public Zombie
{
public:
    Pole_zombie(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :Zombie(imageID, x, y, layer, width, height, animID, world_this)
    {
        change_hp(340);
        change_walking(false);
    };
    virtual void Update()override;
private:
    bool jumped = false;
    bool isrunning = true; //for pole zombie
    bool ishalting = false;
    int  timer = 42;
};
class Bucket_zombie :public Zombie
{
public:
    Bucket_zombie(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID, pGameWorld world_this) :Zombie(imageID, x, y, layer, width, height, animID, world_this)
    {
        change_hp(1300);
    };
    virtual void Update()override;

};
#endif // !GAMEOBJECT_HPP__

