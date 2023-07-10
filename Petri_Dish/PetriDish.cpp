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
namespace m2d
{
    //the ifndef is a temporary solution until I get enough util functions to put them all in a util library
    //for now the function is just pasted wherever needed and has an include guard to avoid conflicts
    #ifndef BETWEEN
    #define BETWEEN
    template <typename T> bool between(T x, T l, T r)
    {
        return ((x >= l) && (x <= r));
    }
    #endif
    std::vector<sf::Vector2u> neighbours4(sf::Vector2u pos, sf::Vector2u dimensions)
    {
        std::vector<sf::Vector2u> out_res;
        //add all pairs of coordiantes that are not out of bounds and have exactly one coordinate changed by +-1
        for(int i = -1; i <= 1; i+=2)
        {
            if(between(pos.x + i, 0u, dimensions.x - 1))
            {
                out_res.push_back({pos.x + i, pos.y});
            }
            if(between(pos.y + i, 0u, dimensions.y - 1))
            {
                out_res.push_back({pos.x, pos.y + i});
            }
        }
        return out_res;
    }
    std::vector<sf::Vector2u> neighbours8(sf::Vector2u pos, sf::Vector2u dimensions)
    {
        std::vector<sf::Vector2u> out_res;
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                if(i == 0 && j == 0) //we only want neighbours, not the tile itself
                {
                    continue;
                }
                if(!between(pos.x + i, 0u, dimensions.x - 1))
                {
                    continue;
                }
                if(!between(pos.y + j, 0u, dimensions.y - 1))
                {
                    continue;
                }
                out_res.push_back({pos.x + i, pos.y + j});
            }
        }
        return out_res;
    }
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
        std::vector<std::vector<cell> > cells;
        std::vector<sf::Vector2u> tiles;
        SpriteSheet* sprite_sheet;
        sf::Vector2u dimensions;
        std::queue<qupdate> updates;
        sf::RenderWindow* dish_window;
        std::mt19937 rng;
        void (*cellProc)(cell c, sf::Vector2u pos, PetriDish* dish);
        void objectInit(SpriteSheet *in_sprite_sheet, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos, PetriDish* dish))
        {
            //basic initialization
            dimensions = in_dimensions;
            cellProc = in_cellProc;
            sprite_sheet = in_sprite_sheet;
            //initializing a matrix of cells
            cells.resize(dimensions.x);
            for(std::vector<cell> &cv : cells)
            {
                cv.resize(dimensions.y);
            }
            //initializing the tiles, could be done without tiles altogether and just a single for loop
            //but I wanted them to be processed in random order in case I ever want to do something that cares about order in which cellProc is called on the cells
            for(unsigned int i = 0; i < dimensions.x; i++)
            {
                for(unsigned int j = 0; j < dimensions.y; j++)
                {
                    tiles.push_back({i, j});
                }
            }
            rng.seed(std::time(NULL));
        }
    public:
        cell& getCell(sf::Vector2u coords)
        {
            return cells[coords.x][coords.y];
        }
        unsigned int randnum()
        {
            return rng();
        }
        void addTask(sf::Vector2u pos, unsigned int type_index)
        {
            //add an update to the update queue which will is resolved at the end of every phase
            updates.push({pos, type_index});
        }
        void addTask(sf::Vector2u pos, std::string type_name)
        {
            addTask(pos, sprite_sheet->getTileIndex(type_name)); //we just change the name-based addTask function to a simple call of the index-based one
        }
        PetriDish(SpriteSheet* in_sprite_sheet, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos, PetriDish* dish))
        {
            objectInit(in_sprite_sheet, in_dimensions, in_cellProc);
        }
        PetriDish(std::string sprite_sheet_name, sf::Vector2u in_spritesize, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos, PetriDish* dish))
        {
            SpriteSheet* new_sprite_sheet = new SpriteSheet(sprite_sheet_name, in_spritesize);
            objectInit(new_sprite_sheet, in_dimensions, in_cellProc);
        }
        PetriDish(std::string sprite_sheet_name, sf::Vector2u in_spritesize, std::string in_dictionary_name, sf::Vector2u in_dimensions, void (*in_cellProc)(cell c, sf::Vector2u pos, PetriDish* dish))
        {
            SpriteSheet* new_sprite_sheet = new SpriteSheet(sprite_sheet_name, in_spritesize, in_dictionary_name);
            objectInit(new_sprite_sheet, in_dimensions, in_cellProc);
        }
        void init(std::vector<std::vector<unsigned int> > &initial_dish, unsigned int ms_tickrate)
        {
            dish_window = new sf::RenderWindow(sf::VideoMode(dimensions.x * sprite_sheet->getSprsize().x, dimensions.y * sprite_sheet->getSprsize().y), "PetriDish"); //create the window
            //fill the cells with proper
            for(unsigned int i = 0; i < dimensions.x; i++)
            {
                for(unsigned int j = 0; j < dimensions.y; j++)
                {
                    cells[i][j].tile_type = initial_dish[i][j];
                    cells[i][j].sprite.setPosition({(float)(1) * i * sprite_sheet->getSprsize().x, (float)(1) * j * sprite_sheet->getSprsize().y});
                    cells[i][j].sprite.setTexture(sprite_sheet->getTexture(cells[i][j].tile_type));
                }
            }
            //main loop of the PetriDish
            while(dish_window->isOpen())
            {
                //handle sfml events, close the window if the 'x' in the right top corner is pressed
                sf::Event event;
                while(dish_window->pollEvent(event))
                {

                    if(event.type == sf::Event::Closed)
                    {
                        dish_window->close();
                    }
                }
                //draw the cells on the screen
                dish_window->clear();
                std::shuffle(tiles.begin(), tiles.end(), rng);
                for(sf::Vector2u cur_tile : tiles)
                {
                    cell &c = cells[cur_tile.x][cur_tile.y];
                    c.sprite.setTexture(sprite_sheet->getTexture(c.tile_type));
                    dish_window->draw(c.sprite);
                }
                dish_window->display();
                //call cellProc for each of the tiles
                for(sf::Vector2u cur_tile : tiles)
                {
                    cellProc(cells[cur_tile.x][cur_tile.y], cur_tile, this);
                }
                //apply the changes queued by the addTask function
                while(!updates.empty())
                {
                    cell &c = cells[updates.front().pos.x][updates.front().pos.y];
                    c.tile_type = updates.front().value;
                    updates.pop();
                }
                std::this_thread::sleep_for(ms_tickrate * 1ms); //I should probably measure the time from last frame and subtract it and make the rendering in a different thread to make the tickspeed consistent
            }

        }
        std::string getName(unsigned int index)
        {
            return sprite_sheet->getName(index);
        }
    };
}
#endif
