#ifndef SPRITEMODULE_H
#define SPRITEMODULE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
namespace m2d
{
    unsigned long long hashFunc(std::string to_hash, unsigned long long hash_base, unsigned long long hash_mod)
    {
        unsigned long long cur_hash = 0;
        for(char c : to_hash)
        {
            cur_hash = (cur_hash + c) % hash_mod;
            cur_hash = (cur_hash * hash_base) % hash_mod;
        }
        return cur_hash;
    }
    class SpriteSheet
    {
        private:
            sf::Vector2u sprite_size;
            sf::Image sheet;
            unsigned int sprite_count;
            std::vector<sf::Texture> textures;
            std::vector<bool> loaded;
            unsigned int hash_base_index = 0;
            std::vector<unsigned int> hash_bases = {733, 739, 743, 751, 757};
            std::map<unsigned long long, int> dictionary;
            void loadTexture(unsigned int index)
            {
                loaded[index] = true;
                unsigned int columns = sheet.getSize().x / sprite_size.x;
                unsigned int column = index % columns;
                unsigned int row = index / columns;
                unsigned int x(index * sprite_size.x), y(row * sprite_size.y);
                textures[index].loadFromImage(sheet, sf::IntRect(x, y, sprite_size.x, sprite_size.y));
            }
            void initDictionary(std::string dictionary_file_name)
            {
                std::vector<std::string> names;
                std::string in;
                std::ifstream dictionary_input(dictionary_file_name);
                while(dictionary_input>>in)
                {
                    names.push_back(in);
                }
                for(unsigned int cur_base : hash_bases)
                {
                    unsigned int counter = 0;
                    dictionary.clear();
                    bool is_collision = false;
                    for(std::string s : names)
                    {
                        long long word_hash = hashFunc(s, cur_base, 1000000007);
                        if(dictionary.find(word_hash) != dictionary.end())
                        {
                            is_collision = true;
                            break;
                        }
                        dictionary.insert({word_hash, counter});
                        counter++;
                    }
                    if(!is_collision)
                    {
                        break;
                    }
                    hash_base_index++;
                }
            }
            void objectInit(std::string file_name, sf::Vector2u sprsize)
            {
                sheet.loadFromFile(file_name);
                sprite_size = sprsize;
                sprite_count = (sheet.getSize().x / sprite_size.x) * (sheet.getSize().y / sprite_size.y);
                textures.resize(sprite_count);
                loaded.resize(sprite_count);
                std::fill(loaded.begin(), loaded.end(), 0);
            }
        public:
            SpriteSheet();
            SpriteSheet(std::string file_name, sf::Vector2u sprsize)
            {
                objectInit(file_name, sprsize);
            }
            SpriteSheet(std::string image_file_name, sf::Vector2u in_sprsize, std::string dictionary_file_name)
            {
                objectInit(image_file_name, in_sprsize);
                initDictionary(dictionary_file_name);
            }
            sf::Vector2u getSize()
            {
                return sheet.getSize();
            }
            sf::Vector2u getSprsize()
            {
                return sprite_size;
            }
            unsigned int getTileIndex(std::string name)
            {
                if(dictionary.find(hashFunc(name, hash_bases[hash_base_index], 1000000007)) == dictionary.end())
                {
                    throw;
                }
                return dictionary[hashFunc(name, hash_bases[hash_base_index], 1000000007)];
            }
            sf::Texture& getTexture(unsigned int index) //TODO: add error handling when reading out of bounds or from uninitialized spritesheet
            {
                if(!loaded[index])
                {
                    loadTexture(index);
                }
                return textures[index];
            }
            sf::Texture& getTexture(std::string name)
            {
                return getTexture(getTileIndex(name));
            }
            bool inDictionary(std::string name)
            {
                return dictionary.find(hashFunc(name, hash_bases[hash_base_index], 1000000007)) != dictionary.end();
            }
    };
}
#endif
