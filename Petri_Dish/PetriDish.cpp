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
    bool between(int x, int l, int r)
    {
        return ((x >= l) && (x <= r));
    }
    vector<Vector2u> neighbours4(Vector2u pos, Vector2u dimensions)
    {

    }
    vector<Vector2u> neighbours8(Vector2u pos, Vector2u dimensions)
    {
        vector<Vector2u> res;
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                if(i == 0 && j == 0)
                {
                    continue;
                }
                if(!between(pos.x + i, 0, dimensions.x - 1))
                {
                    continue;
                }
                if(!between(pos.y + j, 0, dimensions.y - 1))
                {
                    continue;
                }
                res.push_back({pos.x + i, pos.y + j});
            }
        }
        return res;
    }
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
        cell& get_cell(Vector2u coords)
        {
            return cells[coords.x][coords.y];
        }
        unsigned int randnum()
        {
            return rng();
        }
        void addtask(qupdate qup)
        {
            updates.push(qup);
        }
        PetriDish(std::string in_spritesheetname, Vector2u in_spritesize, Vector2u in_dimensions, void (*in_cellproc)(cell c, Vector2u pos))
        {
            dimensions = in_dimensions;
            cellproc = in_cellproc;
            spritesheet = new m2d::SpriteSheet(in_spritesheetname, in_spritesize);
            cells.resize(dimensions.x);
            for(vector<cell> &cv : cells)
            {
                cv.resize(dimensions.y);
            }
            for(unsigned int i = 0; i < dimensions.x; i++)
            {
                for(unsigned int j = 0; j < dimensions.y; j++)
                {
                    tiles.push_back({i, j});
                }
            }
            rng.seed(std::time(NULL));

        } //TODO: alternate constructor that allows for passing an existing spritesheet
        void activate(vector<vector<unsigned int> > &initialdish, unsigned int ms_tickrate)
        {
            dishwindow = new sf::RenderWindow(sf::VideoMode(dimensions.x * spritesheet->getSprsize().x, dimensions.y * spritesheet->getSprsize().y), "PetriDish");
            for(unsigned int i = 0; i < dimensions.x; i++)
            {
                for(unsigned int j = 0; j < dimensions.y; j++)
                {
                    cells[i][j].tiletype = initialdish[i][j];
                    cells[i][j].sprite.setPosition({i * spritesheet->getSprsize().x, j * spritesheet->getSprsize().y});
                    cells[i][j].sprite.setTexture(spritesheet->getTexture(cells[i][j].tiletype)); //todo cellupdate function
                }
            }
            while(dishwindow->isOpen())
            {
                sf::Event event;
                while (dishwindow->pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        dishwindow->close();
                }
                for(Vector2u curtile : tiles)
                {
                    cellproc(cells[curtile.x][curtile.y], curtile);
                }
                while(!updates.empty())
                {
                    cell &c = cells[updates.front().pos.x][updates.front().pos.y];
                    c.tiletype = updates.front().value;

                    updates.pop(); //todo cellupdate function
                }
                dishwindow->clear();
                std::shuffle(tiles.begin(), tiles.end(), rng);
                for(Vector2u curtile : tiles)
                {
                    cell &c = cells[curtile.x][curtile.y];
                    c.sprite.setTexture(spritesheet->getTexture(c.tiletype));
                    dishwindow->draw(c.sprite);
                }
                dishwindow->display();
                std::this_thread::sleep_for(ms_tickrate * 1ms);
            }

        }
    };
}
#endif
