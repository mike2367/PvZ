
#include "GameObject.hpp"
#include"./GameWorld/GameWorld.hpp"
Type GameObject::check_type() const
{
    return type;
}
int GameObject::check_hp() const
{
    return Hp;
}
void GameObject::change_hp(int hp)
{
    Hp = hp;
}
void GameObject::change_type(Type ty)
{
    type = ty;
}
Hits GameObject::check_hit() const
{
    return hit_type;
}
void GameObject::change_hit(Hits type)
{
    hit_type = type;
}

void GameObject::change_walking(bool walking)
{
    iswalking = walking;
}
bool GameObject::check_walking() const
{
    return iswalking;
}
void Background::Update()
{
    return;
}
void Background::OnClick()
{
    return;
}
void PlantingSpot::Update()
{
    return;
}

void PlantingSpot::OnClick()
{
    if (check_hp() == 0)
    {
        return;
    }

    if (p_this->check_current_state() == Holding::ISEMPTY || p_this->check_current_state() == Holding::ISSHOVEL)
    {
        return;
    }
    else
    {
        int x = GetX();
        int y = GetY();
        if (p_this->check_current_state() == Holding::ISPEASHOOTER)
        {
            p_this->pushback(std::make_shared<Peashooter>(IMGID_PEASHOOTER, x, y, LAYER_PLANTS, 60, 80, ANIMID_IDLE_ANIM, p_this));
        }
        else if (p_this->check_current_state() == Holding::ISSUNFLOWER)
        {
            p_this->pushback(std::make_shared<Sunflower>(IMGID_SUNFLOWER, x, y, LAYER_PLANTS, 60, 80, ANIMID_IDLE_ANIM, p_this));
        }
        else if (p_this->check_current_state() == Holding::ISWALLNUT)
        {
            p_this->pushback(std::make_shared<Wallnut>(IMGID_WALLNUT, x, y, LAYER_PLANTS, 60, 80, ANIMID_IDLE_ANIM, p_this));
        }
        else if (p_this->check_current_state() == Holding::ISCHERRY_BOMB)
        {
            p_this->pushback(std::make_shared<Cherry_Bomb>(IMGID_CHERRY_BOMB, x, y, LAYER_PLANTS, 60, 80, ANIMID_IDLE_ANIM, p_this));
        }
        else if (p_this->check_current_state() == Holding::ISREPEATER)
        {
            p_this->pushback(std::make_shared<Repeater>(IMGID_REPEATER, x, y, LAYER_PLANTS, 60, 80, ANIMID_IDLE_ANIM, p_this));
        }
        else
        {
            //holding shovel or empty
            return;
        }
        p_this->change_current_state(Holding::ISEMPTY);

    }
}
void Seed::Update()
{
    return;
}
void Seed::OnClick()
{

    if (p_this->check_current_state() != Holding::ISEMPTY)
    {
        return;
    }
    //the hand is empty
    if (p_this->consume_sun(Cost))
    {
        p_this->change_current_state(whichSeed);
        auto CM = std::make_shared<Cooling_mask>(IMGID_COOLDOWN_MASK, GetX(), GetY(), LAYER_COOLDOWN_MASK, 50, 70, ANIMID_NO_ANIMATION, timer, p_this);
        p_this->pushback(CM);
    }
    else
    {   
        //the sun isn't enough
        return;
    }

}

void Plant::OnClick()
{
    if (p_this->check_current_state() == Holding::ISSHOVEL)
    {
        change_hp(0);
        p_this->change_current_state(Holding::ISEMPTY);
    }
    else
    {
        return;
    }

}
bool Sun::check_source() const
{
    return isnatural;
}
void Sun::Update()
{
    if (check_hp() == 0)
    {
        return;
    }
    if (check_source())
    {
        //coming from the sky
        if (down_timer == 0)
        {
            //awaiting period
            if (disappear_timer == 0)
            {
                change_hp(0);
            }
            else
            {
                disappear_timer--;
            }
        }
        else
        {
            MoveTo(GetX(), GetY() - 2);
            down_timer--;
        }
    }
    else
    {
        //coming from the sunflower
        if (move_timer == 0)
        {
            //awaiting period
            if (disappear_timer == 0)
            {
                change_hp(0);
            }
            else
            {
                disappear_timer--;
            }
        }
        else
        {
            MoveTo(GetX() - 1, GetY() + m_speed_v);
            move_timer--;
        }
    }

    m_speed_v--;
}
void Sun::OnClick()
{
    change_hp(0);
    p_this->SetSun(p_this->GetSun() + 25);
}
void Cooling_mask::Update()
{
    if (timer == 0)
    {
        change_hp(0);
    }
    else
    {
        timer--;
    }
}
void Cooling_mask::OnClick()
{
    return;
}
void Sunflower::get_first_sun_time()
{
    first_sun_time = randInt(30, 600);
}
void Sunflower::Update()
{
    if (check_hp() == 0)
    {
        return;
    }
    if (first_sun_time != 0)
    {
        first_sun_time--;
        return;
    }
    else//the first cooling is over
    {
        //first generate
        if (!generate_first)
        {
            generate_first = true;
            auto sun1 = std::make_shared<Sun>(IMGID_SUN, GetX(), GetY(), LAYER_SUN
                , 80, 80, ANIMID_IDLE_ANIM, false, p_this);
            p_this->pushback(sun1);

        }
        //ongoing
        else
        {
            if (sun_timer == 0)
            {
                sun_timer = 600;//reset cooling time
                auto sun2 = std::make_shared<Sun>(IMGID_SUN, GetX(), GetY(), LAYER_SUN
                    , 80, 80, ANIMID_IDLE_ANIM, false, p_this);
                p_this->pushback(sun2);
                return;
            }
            else
            {
                sun_timer--;
            }
        }
    }
}

void Wallnut::Update()
{
    if (check_hp() == 0)
    {
        return;
    }
    if (check_hp() <= (4000 / 3))
    {
        ChangeImage(IMGID_WALLNUT_CRACKED);
    }

}
void Cherry_Bomb::Update()
{

    if (boom_timer == 0)
    {
        change_hp(0);
        //generate explosion
        auto exp = std::make_shared<Explosion>(IMGID_EXPLOSION, GetX(), GetY(), LAYER_PROJECTILES, 3 * LAWN_GRID_WIDTH, 3 * LAWN_GRID_HEIGHT, ANIMID_NO_ANIMATION, p_this);
        p_this->pushback(exp);
    }
    else
    {
        boom_timer--;
    }
}
void Explosion::Update()
{
    if (timer == 0)
    {
        change_hp(0);
    }
    else
    {
        timer--;
    }
}
void Explosion::OnClick()
{
    return;
}

void Peashooter::Update()
{
    if (check_hp() == 0)
    {
        return;
    }
    if (p_this->check_for_shooter(GetX(), GetY()))
    {
        if (shoot_timer == 0)
        {
            shoot_timer = 30;
            //generate a pea
            auto pea = std::make_shared<Pea>(IMGID_PEA, GetX() + 30, GetY() + 20,
                LAYER_PROJECTILES, 28, 28, ANIMID_NO_ANIMATION, p_this);
            p_this->pushback(pea);
            return;
        }
    }
    if (shoot_timer != 0)
    {
        shoot_timer--;
        return;
    }
}

void Pea::OnClick()
{
    return;
}
void Pea::Update()
{
    if (check_hp() == 0)
    {
        return;
    }
    if (GetX() >= WINDOW_WIDTH)
    {
        change_hp(0);
        return;
    }
    MoveTo(GetX() + 8, GetY());

}
void Repeater::Update()
{
    if (check_hp() == 0)
    {
        return;
    }
    if (p_this->check_for_shooter(GetX(), GetY()))
    {
        if (shoot_timer == 0)
        {
            auto p1 = std::make_shared<Pea>(IMGID_PEA, GetX() + 30, GetY() + 20,
                LAYER_PROJECTILES, 28, 28, ANIMID_NO_ANIMATION, p_this);
            p_this->pushback(p1);
            shoot_timer = 30;
            //begin the second timing
            second_start = true;
            return;
        }
        if (second_timer == 0)
        {
            auto p2 = std::make_shared<Pea>(IMGID_PEA, GetX() + 30, GetY() + 20,
                LAYER_PROJECTILES, 28, 28, ANIMID_NO_ANIMATION, p_this);
            p_this->pushback(p2);
            second_timer = 5;
            //finish the second timing
            second_start = false;
            return;
        }

    }
    if (second_start)
    {
        second_timer--;
        shoot_timer--;
        return;
    }
    if (shoot_timer != 0)
    {
        shoot_timer--;
    }
}

void Shovel::Update()
{
    return;
}
void Shovel::OnClick()
{
    if (p_this->check_current_state() == Holding::ISSHOVEL)
    {
        p_this->change_current_state(Holding::ISEMPTY);
    }
    else if (p_this->check_current_state() == Holding::ISEMPTY)
    {
        p_this->change_current_state(Holding::ISSHOVEL);
    }
    else
    {
        return;
    }
}
void Zombie::Update()
{
    if (check_hp() == 0)
    {
        return;
    }
    if (check_walking())
    {
        MoveTo(GetX() - 1, GetY());
    }

}
void Zombie::OnClick()
{
    return;
}

void Pole_zombie::Update()
{
    if (check_hp() == 0)
    {
        return;
    }
    //plants ahead and haven't jumped
    if (p_this->check_for_pole_zombie(GetX(), GetY()) && !jumped)
    {
        PlayAnimation(ANIMID_JUMP_ANIM);
        ishalting = true;
        jumped = true;
        isrunning = false;
    }
    //walking
    if (check_walking())
    {
       MoveTo(GetX() - 1, GetY());
    }
    else if (isrunning)
    {
        MoveTo(GetX() - 2, GetY());
        return;
    }
    else if (ishalting)
    {
        if (timer == 0)
        {
            MoveTo(GetX() - 150, GetY());
            ishalting = false;
            change_walking(true);
            PlayAnimation(ANIMID_WALK_ANIM);
        }
        else
        {
            timer--;
        }
        return;
    }
}
void Bucket_zombie::Update()
{
    if (check_hp() == 0)
    {
        return;
    }
    if (check_hp() <= 200)
    {
        ChangeImage(IMGID_REGULAR_ZOMBIE);
    }
    if (check_walking())
    {
        MoveTo(GetX() - 1, GetY());
    }
}







