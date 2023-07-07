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
namespace msim
{
    bool between(int x, int l, int r);
    vector<Vector2u> neighbours4(Vector2u pos, Vector2u dimensions);
    vector<Vector2u> neighbours8(Vector2u pos, Vector2u dimensions);
    struct qupdate
    {
        Vector2u pos;
        unsigned int value;
    };
    struct cell
    {
        unsigned int tiletype;
        sf::Sprite sprite;
    };
    class PetriDish
    {
    private:
        vector<vector<cell> > cells;
        vector<Vector2u> tiles;
        m2d::SpriteSheet* spritesheet;
        Vector2u dimensions;
        std::queue<qupdate> updates;
        sf::RenderWindow* dishwindow;
        std::mt19937 rng;
        void (*cellproc)(cell c, Vector2u pos);
    public:
        cell& get_cell(Vector2u coords);
        unsigned int randnum();
        void addtask(qupdate qup);
        PetriDish(std::string in_spritesheetname, Vector2u in_spritesize, Vector2u in_dimensions, void (*in_cellproc)(cell c, Vector2u pos)); //TODO: alternate constructor that allows for passing an existing spritesheet
        void activate(vector<vector<unsigned int> > &initialdish, unsigned int mstickrate);
    };
}
#endif
