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
    #ifndef BETWEEN
    #define BETWEEN
    template <typename T> bool between(T x, T l, T r);
    #endif
    std::vector<sf::Vector2u> neighbours4(sf::Vector2u pos, sf::Vector2u dimensions);
    std::vector<sf::Vector2u> neighbours8(sf::Vector2u pos, sf::Vector2u dimensions);
    struct qupdate
    {
        sf::Vector2u pos;
        unsigned int index;
        string name;
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
        std::queue<qupdate*> updates;
        sf::RenderWindow* dish_window;
        std::mt19937 rng;
        void (*cellProc)(cell c, sf::Vector2u pos);
        void objectInit(SpriteSheet* in_sprite_sheet, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos));
    public:
        cell& getCell(sf::Vector2u coords);
        unsigned int randnum();
        void addTask(qupdate qup);
        PetriDish(SpriteSheet *in_sprite_sheet, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos));
        PetriDish(std::string sprite_sheet_name, sf::Vector2u in_spritesize, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos));
        PetriDish(std::string sprite_sheet_name, sf::Vector2u in_spritesize, std::string in_dictionary_name, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos))
        void init(std::vector<std::vector<unsigned int> > &initial_dish, unsigned int ms_tickrate);
    };
}
#endif
