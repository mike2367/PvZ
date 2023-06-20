#ifndef GAMEWORLD_HPP__
#define GAMEWORLD_HPP__

#include <list>
#include <memory>

#include "./Framework/WorldBase.hpp"
#include "utils.hpp"
#include "./GameObject/GameObject.hpp"


class GameWorld : public WorldBase, public std::enable_shared_from_this<GameWorld> {
public:
    // Use shared_from_this() instead of "this".
    using std::enable_shared_from_this<GameWorld>::shared_from_this;
    GameWorld();
    virtual ~GameWorld();

    void Init() override;

    LevelStatus Update() override;

    void CleanUp() override;

    Holding check_current_state() const;

    void change_current_state(Holding state);

    bool consume_sun(int num);

    void pushback(std::shared_ptr<GameObject> gm);

    bool ishit(std::list<std::shared_ptr<GameObject>>::iterator gm1, std::list<std::shared_ptr<GameObject>>::iterator gm2);

    void set_planting_spots();

    void generate_seeds();

    void generate_sun();

    void generate_zombies();

    bool check_for_shooter(int x, int y);

    bool check_for_pole_zombie(int x, int y);

    void check_hit_and_upgrade();

    void final_check();

    LevelStatus check_death_and_winning();

private:
    int first_wave = 1200;
    int wave_timer = std::max(150, 600 - 20 * GetWave());
    Holding current_state = Holding::ISEMPTY;
    std::list<std::shared_ptr<GameObject>> game;
    int first_timer = 180;
    int normal_timer = 300;
    int generate_round = 1;
};


#endif // !GAMEWORLD_HPP__
