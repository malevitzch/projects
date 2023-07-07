#ifndef SPRITEMODULE_H
#define SPRITEMODULE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
namespace m2d
{
    using sf::Texture, sf::Image, sf::Sprite, sf::Vector2u, sf::IntRect;
    unsigned long long hashFunc(std::string _tohash, unsigned long long _hashbase, unsigned long long _hashmod)
    {
        unsigned long long _hash = 0;
        for(char _c : _tohash)
        {
            _hash = (_hash + _c) % _hashmod;
            _hash = (_hash * _hashbase) % _hashmod;
        }
        return _hash;
    }
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
            void loadTexture(unsigned int _no)
            {
                loaded[_no] = true;
                unsigned int _columns = getSize().x / spritesize.x; //can be done with a lot less math
                unsigned int _column = _no % _columns;
                unsigned int _row = _no / _columns;
                unsigned int _x(_column * spritesize.x), _y(_row * spritesize.y);
                textures[_no].loadFromImage(sheet, sf::IntRect(_x, _y, spritesize.x, spritesize.y));
            }
            void initDictionary(std::string _dictionaryfilename)
            {
                std::vector<std::string> _names;
                std::string _in;
                std::ifstream _dictionaryinput(_dictionaryfilename);
                while(_dictionaryinput>>_in)
                {
                    _names.push_back(_in);
                }
                for(unsigned int _curbase : hashbases)
                {
                    unsigned int counter = 0;
                    _dictionary.clear();
                    bool good = true;
                    for(std::string _s : _names)
                    {
                        long long _wordhash = hashFunc(_s, _curbase, 1000000007);
                        if(_dictionary.find(_wordhash) != _dictionary.end())
                        {
                            good = false;
                            break;
                        }
                        _dictionary.insert({_wordhash, counter});
                        counter++;
                    }
                    if(good)
                    {
                        break;
                    }
                    hashmodind++;
                }
            }
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
            SpriteSheet(std::string _imagefilename, Vector2u _sprsize, std::string _dictionaryfilename)
            {
                SpriteSheet(_imagefilename, _sprsize);
                initDictionary(_dictionaryfilename);
            }
            Vector2u getSize()
            {
                return sheet.getSize();
            }
            Vector2u getSprsize()
            {
                return spritesize;
            }
            Texture& getTexture(unsigned int _no) //TODO: add error handling when reading out of bounds or from uninitialized spritesheet
            {
                if(!loaded[_no])
                {
                    loadTexture(_no);
                }
                return textures[_no];
            }
            Texture& getTexture(std::string _name)
            {
                return getTexture(_dictionary[hashFunc(_name, hashbases[hashmodind], 1000000007)]);
            }
    };
}
#endif
