#include "PetriDish.cpp"
#ifndef H_PETRIDISH
#define H_PETRIDISH
#include "../Spritesheet_Lib/SpriteModule.h"
#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;
using std::vector;
using sf::Vector2u;
namespace m2d
{
    bool between(int in_x, int in_l, int in_r);
    std::vector<sf::Vector2u> neighbours4(sf::Vector2u pos, sf::Vector2u dimensions);
    std::vector<sf::Vector2u> neighbours8(sf::Vector2u pos, sf::Vector2u dimensions);
    struct qupdate
    {
        sf::Vector2u pos;
        unsigned int value;
    };
    struct cell
    {
        unsigned int tile_type;
        sf::Sprite sprite;
    };
    class PetriDish
    {
    private:
        std::vector<vector<cell> > cells;
        std::vector<Vector2u> tiles;
        SpriteSheet* sprite_sheet;
        sf::Vector2u dimensions;
        std::queue<qupdate> updates;
        sf::RenderWindow* dish_window;
        std::mt19937 rng;
        void (*cellProc)(cell c, sf::Vector2u pos);
    public:
        cell& getCell(sf::Vector2u coords);
        unsigned int randnum();
        void addTask(qupdate qup);
        PetriDish(std::string in_spritesheetname, sf::Vector2u in_spritesize, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos)); //TODO: alternate constructor that allows for passing an existing spritesheet
        void init(std::vector<std::vector<unsigned int> > &initial_dish, unsigned int ms_tickrate);
    };
}
#endif
