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
    template <typename T> bool between(T x, T l, T r); //simple function template that checks whether a value is inside a certain range, inclusive
    #endif
    std::vector<sf::Vector2u> neighbours4(sf::Vector2u pos, sf::Vector2u dimensions); //returns a vector of coordinates of neighbouring cells (east west north south), does a bounds check to see if the cells exist
    std::vector<sf::Vector2u> neighbours8(sf::Vector2u pos, sf::Vector2u dimensions); //returns a vector of coordinates of neighbouring cells (all 8 neighbouring cells), does a bounds check to see if the cells exist
    struct qupdate //struct used by the PetriDish class to allow user-made cellProc functions to modify the cells
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
        void (*cellProc)(cell c, sf::Vector2u pos, PetriDish* dish);
        void objectInit(SpriteSheet* in_sprite_sheet, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos, PetriDish* dish));
    public:
        cell& getCell(sf::Vector2u coords); //returns cell with the chosen coordinates
        unsigned int randnum(); //returns random number
        void addTask(qupdate qup); //adds a task to the update queue, recommended to use when making the cellProc function
        PetriDish(SpriteSheet *in_sprite_sheet, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos));
        PetriDish(std::string sprite_sheet_name, sf::Vector2u in_spritesize, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos, PetriDish* dish));
        PetriDish(std::string sprite_sheet_name, sf::Vector2u in_spritesize, std::string in_dictionary_name, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos, PetriDish* dish))
        void init(std::vector<std::vector<unsigned int> > &initial_dish, unsigned int ms_tickrate); //function used to initialize the dish, takes a matrix that specifies the starting states of cells and a tickrate value in miliseconds as input
        std::string getName(unsigned int index); //returns the name of tile with chosen index
    };
}
#endif
