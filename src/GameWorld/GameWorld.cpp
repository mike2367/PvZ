#include "GameWorld.hpp"

GameWorld::GameWorld() {  }

GameWorld::~GameWorld() { }

Holding GameWorld::check_current_state() const
{
    return current_state;
}
void GameWorld::change_current_state(Holding state)
{
    current_state = state;
}
void GameWorld::pushback(std::shared_ptr<GameObject> gm)
{
    game.push_back(gm);
}
bool GameWorld::consume_sun(int num)
{
    int c_sun = GetSun();//current sun
    if (num <= c_sun)
    {
        SetSun(c_sun - num);
        return true;// successfully use a card
    }
    else
    {
        return false;
    }
}
bool GameWorld::ishit(std::list<std::shared_ptr<GameObject>>::iterator gm1, std::list<std::shared_ptr<GameObject>>::iterator gm2)
{
    if (abs((*gm1)->GetX() - (*gm2)->GetX()) <= ((*gm1)->GetWidth() + (*gm2)->GetWidth()) / 2 &&
        abs((*gm1)->GetY() - (*gm2)->GetY()) <= ((*gm1)->GetHeight() + (*gm2)->GetHeight()) / 2 &&
        ((*gm1)->check_type() == Type::ZOMBIE))
    {
        return true;
    }
    return false;
}
//check hits
void GameWorld::check_hit_and_upgrade()
{
    for (auto it = game.begin(); it != game.end(); it++)
    {
        (*it)->Update();
        if ((*it)->check_type() == Type::ZOMBIE)
        {
            for (auto it2 = game.begin(); it2 != game.end(); it2++)
            {
                if (ishit(it, it2))
                {
                    if ((*it2)->check_type() == Type::PEA)
                    {
                        (*it2)->change_hp(0);
                        (*it)->change_hp((*it)->check_hp() - 20);
                    }
                    if ((*it2)->check_type() == Type::EXPLOSION)
                    {
                        (*it)->change_hp(0);
                    }
                    if ((*it2)->check_type() == Type::PLANT)
                    {
                        if ((*it)->check_walking())
                        {
                            (*it)->PlayAnimation(ANIMID_EAT_ANIM);
                            (*it)->change_walking(false);
                        }
                        else if ((*it2)->check_hp() <= 0)
                        {
                            (*it)->PlayAnimation(ANIMID_WALK_ANIM);
                            (*it)->change_walking(true);
                        }
                        else
                        {
                            if ((*it2)->check_hp() <= 3)
                            {
                                (*it)->PlayAnimation(ANIMID_WALK_ANIM);
                                (*it)->change_walking(true);
                            }
                            (*it2)->change_hp((*it2)->check_hp() - 3);

                        }
                    }
                }
            }
        }
    }
}
void GameWorld::final_check()
{
    //check one more time for zombie eating
    for (auto it = game.begin(); it != game.end(); it++)
    {
        if ((*it)->check_type() == Type::ZOMBIE)
        {
            for (auto it2 = it; it2 != game.end(); it2++)
            {
                if ((*it2)->check_type() == Type::PLANT)
                {
                    if (ishit(it, it2))
                    {
                        if (!(*it)->check_walking() && (*it2)->check_hp() == 0)
                        {
                            (*it)->change_walking(true);
                            (*it)->PlayAnimation(ANIMID_WALK_ANIM);
                        }
                    }

                }

            }
        }
    }
}

void GameWorld::set_planting_spots()
{
    int x = FIRST_COL_CENTER;
    int y = FIRST_ROW_CENTER;
    //set planting spot
    for (int i = 0; i < GAME_COLS; i++)
    {
        y = FIRST_ROW_CENTER;
        for (int j = 0; j < GAME_ROWS; j++)
        {
            auto plantingspot = std::make_shared<PlantingSpot>(IMGID_NONE, x, y, LAYER_UI, 60, 80, ANIMID_NO_ANIMATION, shared_from_this());
            y += LAWN_GRID_HEIGHT;
            game.push_back(plantingspot);
        }
        x += LAWN_GRID_WIDTH;

    }
}
void GameWorld::generate_seeds()
{
    auto  sunflower_seed = std::make_shared<Sunflower_Seed>(IMGID_SEED_SUNFLOWER, 130, WINDOW_HEIGHT - 44, LAYER_UI, 50, 70, ANIMID_NO_ANIMATION, 50, Holding::ISSUNFLOWER, 240, shared_from_this());
    auto  peashooter_seed = std::make_shared<Peashooter_Seed>(IMGID_SEED_PEASHOOTER, 190, WINDOW_HEIGHT - 44, LAYER_UI, 50, 70, ANIMID_NO_ANIMATION, 100, Holding::ISPEASHOOTER, 240, shared_from_this());
    auto  wallnut_seed = std::make_shared<Wallnut_Seed>(IMGID_SEED_WALLNUT, 250, WINDOW_HEIGHT - 44, LAYER_UI, 50, 70, ANIMID_NO_ANIMATION, 50, Holding::ISWALLNUT, 900, shared_from_this());
    auto  cherry_bomb_seed = std::make_shared<Cherry_bomb_Seed>(IMGID_SEED_CHERRY_BOMB, 310, WINDOW_HEIGHT - 44, LAYER_UI, 50, 70, ANIMID_NO_ANIMATION, 150, Holding::ISCHERRY_BOMB, 1200, shared_from_this());
    auto  repeater_seed = std::make_shared<Repeater_seed>(IMGID_SEED_REPEATER, 370, WINDOW_HEIGHT - 44, LAYER_UI, 50, 70, ANIMID_NO_ANIMATION, 200, Holding::ISREPEATER, 240, shared_from_this());
    auto  shovel = std::make_shared<Shovel>(IMGID_SHOVEL, 600, WINDOW_HEIGHT - 40, LAYER_UI, 50, 50, ANIMID_NO_ANIMATION, shared_from_this());
    game.push_back(sunflower_seed);
    game.push_back(peashooter_seed);
    game.push_back(wallnut_seed);
    game.push_back(cherry_bomb_seed);
    game.push_back(repeater_seed);
    game.push_back(shovel);
}

void GameWorld::generate_sun()
{
    //generate sun

    if (normal_timer == 0)//normally generate
    {
        int sun_x = randInt(75, WINDOW_WIDTH - 75);
        int sun_y = WINDOW_HEIGHT - 1;
        auto sun = std::make_shared<Sun>(IMGID_SUN, sun_x, sun_y, LAYER_SUN, 80, 80, ANIMID_IDLE_ANIM, true, shared_from_this());
        game.push_back(sun);
        normal_timer = 300;
    }
    else
    {
        normal_timer--;
    }

}
void GameWorld::generate_zombies()
{
    //generate zombies
    //preparation
    if (first_timer != 0)
    {
        first_timer--;
    }
    else
    {
        if (wave_timer == 0)
        {
            SetWave(GetWave() + 1);
        }
        else
        {
            wave_timer--;
            return;
            return;
        }
        int total = floor(15 + GetWave()) / 10;
        int p1 = 20;
        int p2 = 2 * std::max(GetWave() - 8, 0);
        int p3 = 3 * std::max(GetWave() - 15, 0);
        int normal_poss = p1 / (double)(p1 + p2 + p3) * 100;
        int pole_poss = p2 / (double)(p1 + p2 + p3) * 100;
        int bucket_poss = p3 / (double)(p1 + p2 + p3) * 100;

        //make_zombie
        if (wave_timer == 0)
        {
            for (int i = 0; i < total; i++)
            {
                int zombie_x = randInt(WINDOW_WIDTH - 40, WINDOW_WIDTH - 1);
                int rand = randInt(0, 4);
                int zombie_y = FIRST_ROW_CENTER + rand * LAWN_GRID_HEIGHT;

                int random = randInt(1, normal_poss + pole_poss + bucket_poss);
                if (random <= normal_poss)
                {
                    auto zombie = std::make_shared<Zombie>(IMGID_REGULAR_ZOMBIE, zombie_x, zombie_y, LAYER_ZOMBIES, 20, 80, ANIMID_WALK_ANIM, shared_from_this());
                    game.push_back(zombie);
                }
                else if (random <= normal_poss + pole_poss)
                {

                    auto pole_zombie = std::make_shared<Pole_zombie>(IMGID_POLE_VAULTING_ZOMBIE, zombie_x, zombie_y, LAYER_ZOMBIES, 20, 80, ANIMID_RUN_ANIM, shared_from_this());
                    game.push_back(pole_zombie);
                }
                else
                {
                    auto bucket_zombie = std::make_shared<Bucket_zombie>(IMGID_BUCKET_HEAD_ZOMBIE, zombie_x, zombie_y, LAYER_ZOMBIES, 20, 80, ANIMID_WALK_ANIM, shared_from_this());
                    game.push_back(bucket_zombie);
                }
            }
            wave_timer = std::max(150, 600 - 20 * GetWave());
        }
    }


}
bool GameWorld::check_for_shooter(int x, int y)
{
    for (auto it = game.begin(); it != game.end(); it++)
    {
        if ((*it)->check_type() == Type::ZOMBIE)
        {
            if ((*it)->GetY() == y && (*it)->GetX() >= x)
            {
                return true;
            }
        }
    }
    return false;
}
bool GameWorld::check_for_pole_zombie(int x, int y)
{
    for (auto it = game.begin(); it != game.end(); it++)
    {
        if ((*it)->check_type() == Type::PLANT)
        {
            if (x - 40 - (*it)->GetX() <= 40 && x - 40 - (*it)->GetX() >= 0)
            {
                return true;
            }
        }

    }
    return false;
}
LevelStatus GameWorld::check_death_and_winning()
{
    for (auto it = game.begin(); it != game.end(); )
    {
        if ((*it)->check_hp() <= 0)
        {
            game.erase(it++);
        }
        //living zombies enter
        else if ((*it)->check_type() == Type::ZOMBIE)
        {
            if ((*it)->GetX() < 0)
            {
                return LevelStatus::LOSING;
            }

            it++;
        }
        else
        {
            it++;
        }

    }
    return LevelStatus::ONGOING;
}
void GameWorld::Init() {
    // YOUR CODE HERE
    SetWave(0);
    SetSun(50);

    normal_timer = 180;

    //set background
    std::shared_ptr<GameObject> background = std::make_shared<Background>(IMGID_BACKGROUND, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, LAYER_BACKGROUND, WINDOW_WIDTH, WINDOW_HEIGHT, ANIMID_NO_ANIMATION, shared_from_this());
    game.push_back(background);

    set_planting_spots();

    generate_seeds();

}

LevelStatus GameWorld::Update() {
    // YOUR CODE HERE

    generate_sun();

    generate_zombies();

    check_hit_and_upgrade();

    final_check();

    //check death and winning
    LevelStatus LS = check_death_and_winning();

    return LS;

}

void GameWorld::CleanUp() {
    // YOUR CODE HERE
    for (auto it = game.begin(); it != game.end();)
    {
        game.erase(it++);
    }
}

