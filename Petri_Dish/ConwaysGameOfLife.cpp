#include "PetriDish.h"
#include <bits/stdc++.h>
m2d::PetriDish* game_dish;
m2d::SpriteSheet* new_sprite_sheet;
void conwaysProc(m2d::cell c, sf::Vector2u pos)
{
    int live_neighbours = 0;
    for(sf::Vector2u cur_cell: m2d::neighbours8(pos, {32, 32}))
    {
        if(new_sprite_sheet->getName(game_dish->getCell(cur_cell).tile_type) == "ALIVE")
        {
            live_neighbours++;
        }
    }
    if(new_sprite_sheet->getName(c.tile_type) == "ALIVE")
    {
        if(!m2d::between(live_neighbours, 2, 3))
        {
            game_dish->addTask(pos, "DEAD");
        }
    }
    else
    {
        if(live_neighbours == 3)
        {
            game_dish->addTask(pos, "ALIVE");
        }
    }
}
int main()
{
    new_sprite_sheet = new m2d::SpriteSheet("Assets.png", {16, 16}, "names");
    game_dish = new m2d::PetriDish(new_sprite_sheet, {32, 32}, conwaysProc);
    std::vector<std::vector<unsigned int> > initial_dish;
    initial_dish.resize(32);
    for(std::vector<unsigned int> &cur_vec : initial_dish)
    {
        cur_vec.resize(32, 0);
    }
    initial_dish[12][16] = 1;
    initial_dish[13][16] = 1;
    initial_dish[13][17] = 1;
    initial_dish[17][17] = 1;
    initial_dish[18][17] = 1;
    initial_dish[18][19] = 1;
    initial_dish[19][17] = 1;
    game_dish->init(initial_dish, 100);
}
