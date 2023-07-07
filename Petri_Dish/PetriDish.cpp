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
    bool between(int in_x, int in_l, int in_r)
    {
        return ((x >= l) && (x <= r));
    }
    vector<Vector2u> neighbours4(Vector2u in_pos, Vector2u in_dimensions)
    {
        vector<Vector2u> out_res;
        for(int i = -1; i <= 1; i+=2)
        {
            if(between(in_pos.x + i, 0, in_dimensions.x - 1))
            {
                out_res.push_back({in_pos.x + i, in_pos.y});
            }
            if(between(in_pos.y + i, 0, in_dimensions.y - 1))
            {
                out_res.push_back({in_pos.x, in_pos.y + i});
            }
        }
        return out_res;
    }
    vector<Vector2u> neighbours8(Vector2u in_pos, Vector2u in_dimensions)
    {
        vector<Vector2u> out_res;
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                if(i == 0 && j == 0)
                {
                    continue;
                }
                if(!between(in_pos.x + i, 0, in_dimensions.x - 1))
                {
                    continue;
                }
                if(!between(in_pos.y + j, 0, in_dimensions.y - 1))
                {
                    continue;
                }
                out_res.push_back({in_pos.x + i, in_pos.y + j});
            }
        }
        return out_res;
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
        void (*cellproc)(cell in_c, Vector2u in_pos);
    public:
        cell& get_cell(Vector2u in_coords)
        {
            return cells[in_coords.x][in_coords.y];
        }
        unsigned int randnum()
        {
            return rng();
        }
        void addtask(qupdate in_qup)
        {
            updates.push(in_qup);
        }
        PetriDish(std::string in_spritesheetname, Vector2u in_spritesize, Vector2u in_dimensions, void (*in_cellproc)(cell in_c, Vector2u in_pos))
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

        }
        void activate(vector<vector<unsigned int> > &in_initialdish, unsigned int in_mstickrate)
        {
            dishwindow = new sf::RenderWindow(sf::VideoMode(dimensions.x * spritesheet->getSprsize().x, dimensions.y * spritesheet->getSprsize().y), "PetriDish");
            for(unsigned int i = 0; i < dimensions.x; i++)
            {
                for(unsigned int j = 0; j < dimensions.y; j++)
                {
                    cells[i][j].tiletype = in_initialdish[i][j];
                    cells[i][j].sprite.setPosition({(float)(1) * i * spritesheet->getSprsize().x, (float)(1) * j * spritesheet->getSprsize().y});
                    cells[i][j].sprite.setTexture(spritesheet->getTexture(cells[i][j].tiletype));
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
                for(Vector2u t_curtile : tiles)
                {
                    cellproc(cells[t_curtile.x][t_curtile.y], t_curtile);
                }
                while(!updates.empty())
                {
                    cell &t_c = cells[updates.front().pos.x][updates.front().pos.y];
                    t_c.tiletype = updates.front().value;
                    updates.pop();
                }
                dishwindow->clear();
                std::shuffle(tiles.begin(), tiles.end(), rng);
                for(Vector2u curtile : tiles)
                {
                    cell &t_c = cells[curtile.x][curtile.y];
                    t_c.sprite.setTexture(spritesheet->getTexture(t_c.tiletype));
                    dishwindow->draw(t_c.sprite);
                }
                dishwindow->display();
                std::this_thread::sleep_for(in_ms_tickrate * 1ms);
            }

        }
    };
}
#endif
