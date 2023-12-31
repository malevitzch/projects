#include "PetriDish.h"
#include <bits/stdc++.h>
void conwaysProc(m2d::cell c, sf::Vector2u pos, m2d::PetriDish* dish)
{
    int live_neighbours = 0;
    //count 'alive' neighbours
    for(sf::Vector2u cur_cell: m2d::neighbours8(pos, {32, 32}))
    {
        if(dish->getName(dish->getCell(cur_cell).tile_type) == "ALIVE")
        {
            live_neighbours++;
        }
    }
    if(dish->getName(c.tile_type) == "ALIVE")
    {
        //an alive cell survives if it has 2 or 3 neighbours
        if(!m2d::between(live_neighbours, 2, 3))
        {
            dish->addTask(pos, "DEAD");
        }
    }
    else
    {
        //a new cell is created if there are exactly 3 neighbours
        if(live_neighbours == 3)
        {
            dish->addTask(pos, "ALIVE");
        }
    }
}
int main()
{
    m2d::SpriteSheet* new_sprite_sheet = new m2d::SpriteSheet("Assets.png", {16, 16}, "names");
    m2d::PetriDish* game_dish = new m2d::PetriDish(new_sprite_sheet, {32, 32}, conwaysProc);
    std::vector<std::vector<unsigned int> > initial_dish;
    initial_dish.resize(32);
    for(std::vector<unsigned int> &cur_vec : initial_dish)
    {
        cur_vec.resize(32, 0);
    }
    //creating the inital pattern
    initial_dish[12][16] = 1;
    initial_dish[13][16] = 1;
    initial_dish[13][17] = 1;
    initial_dish[17][17] = 1;
    initial_dish[18][17] = 1;
    initial_dish[18][19] = 1;
    initial_dish[19][17] = 1;
    game_dish->init(initial_dish, 100);
}
