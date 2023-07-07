#include "SpriteModule.cpp"
#ifndef SPRITEMODULE_H
#define SPRITEMODULE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
namespace m2d
{
    unsigned long long hashFunc(std::string _toHash, unsigned long long _hashbase, unsigned long long _hashmod);
    class SpriteSheet
    {
        private:
            Vector2u spritesize;
            Image sheet;
            unsigned int spritecount;
            std::vector<Texture> textures;
            std::vector<bool> loaded;
            unsigned int hashmodind = 0;
            std::vector<unsigned int> hashbases = {733, 739, 743, 751, 757};
            std::map<unsigned long long, int> _dictionary;
            void loadTexture(unsigned int _no);
            void initDictionary(std::string _disctionaryfilename);
        public:
            SpriteSheet();
            SpriteSheet(std::string _filename, Vector2u _sprsize);
            SpriteSheet(std::string _imagefilename, Vector2u _sprsize, std::string _dictionaryfilename);
            Vector2u getSize();
            Vector2u getSprsize();
            Texture& getTexture(unsigned int _no);
            Texture& getTexture(std::string _name);
    };
}
#endif