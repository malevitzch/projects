#include "SpriteModule.cpp"
#ifndef SPRITEMODULE_H
#define SPRITEMODULE_H
#include <SFML/Graphics.hpp>
#include <iostream>

namespace m2d
{
    class SpriteSheet
    {
        private:
            Vector2u spritesize;
            Image sheet;
            unsigned int spritecount;
            std::vector<Texture> textures;
            std::vector<bool> loaded;
            std::map<long long, int> spritehash;
        public:
            SpriteSheet();
            SpriteSheet(std::string _filename, Vector2u _sprsize);
            Vector2u getSize();
            Vector2u getSprsize();
            void loadTexture(unsigned int _no);
            Texture& getTexture(unsigned int _no);
    };
}
#endif
