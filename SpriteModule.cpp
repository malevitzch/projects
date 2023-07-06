#ifndef SPRITEMODULE_H
#define SPRITEMODULE_H
#include <SFML/Graphics.hpp>
#include <iostream>
namespace m2d
{
    using sf::Texture, sf::Image, sf::Sprite, sf::Vector2u, sf::IntRect;
    class SpriteSheet
    {
        private:
            Vector2u spritesize;
            Image sheet;
            unsigned int spritecount;
            std::vector<Texture> textures;
            std::vector<bool> loaded;
            std::map<long long, int> m;
        public:
            SpriteSheet();
            SpriteSheet(std::string _filename, sf::Vector2u _sprsize)
            {
                sheet.loadFromFile(_filename);
                spritesize = _sprsize;
                spritecount = (sheet.getSize().x / spritesize.x) * (sheet.getSize().y / spritesize.y);
                textures.resize(spritecount);
                loaded.resize(spritecount);
                std::fill(loaded.begin(), loaded.end(), 0);
            }
            Vector2u getSize()
            {
                return sheet.getSize();
            }
            Vector2u getSprsize()
            {
                return spritesize;
            }
            void loadTexture(unsigned int _no)
            {
                loaded[_no] = true;
                unsigned int _columns = getSize().x / spritesize.x; //can be done with a lot less math
                unsigned int _column = _no % _columns;
                unsigned int _row = _no / _columns;
                unsigned int _x(_column * spritesize.x), _y(_row * spritesize.y);
                textures[_no].loadFromImage(sheet, sf::IntRect(_x, _y, spritesize.x, spritesize.y));
            }
            Texture& getTexture(unsigned int _no) //TODO: add error handling when reading out of bounds or from uninitialized spritesheet
            {
                if(!loaded[_no])
                {
                    loadTexture(_no);
                }
                return textures[_no];
            }
    };
}
#endif
