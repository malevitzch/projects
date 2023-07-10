#ifndef SPRITEMODULE_H
#define SPRITEMODULE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
namespace m2d
{
    unsigned long long hashFunc(std::string to_hash, unsigned long long hash_base, unsigned long long hash_mod) //plain old polynomial hash function that hashes strings
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
            std::vector<std::string> names;
            unsigned int error_tile_index = 0;
            void loadTexture(unsigned int index)
            {
                loaded[index] = true;
                //using math to determine the position of the texture in the sheet image
                unsigned int columns = sheet.getSize().x / sprite_size.x;
                unsigned int column = index % columns;
                unsigned int row = index / columns;
                unsigned int x(index * sprite_size.x), y(row * sprite_size.y);
                textures[index].loadFromImage(sheet, sf::IntRect(x, y, sprite_size.x, sprite_size.y));
            }
            void initDictionary(std::string dictionary_file_name)
            {
                std::string in;
                std::ifstream dictionary_input(dictionary_file_name);
                while(dictionary_input>>in)
                {
                    names.push_back(in);
                }
                //we iterate through five potential hash bases until we find one that will cause no hash collisions
                //with a 1e9 + 7 modulo and 5 tries the chances of not getting a collisionless initialisation are abysmal
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
                        dictionary.insert({word_hash, counter}); //we insert the hashes into a map that later allows us to get the tile by name using getTileIndex
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
                //base initialisation of a SpriteSheet
                sheet.loadFromFile(file_name);
                sprite_size = sprsize;
                if(sheet.getSize().x % sprite_size.x != 0 && sheet.getSize().y % sprite_size.y != 0)
                {
                    printf("the spritesheet needs to be evenly divided into tiles\n");
                    throw;
                }
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
                //throw an exception in case we try to access a tile that doesn't exist
                //perhaps try catch would be a better idea here so that we don't have to crash the program
                unsigned int out_ans = 0;
                try
                {
                    out_ans = dictionary.at(hashFunc(name, hash_bases[hash_base_index], 1000000007)); //the map::at function throws an exception in case of non-existent key
                }
                catch(...)
                {
                    printf("unknown tile name \"%s\" \n", name.c_str()); //output the tile name that caused the error
                    out_ans = error_tile_index;
                }
                return out_ans;
            }
            sf::Texture& getTexture(unsigned int index) //TODO: add error handling when reading out of bounds or from uninitialized spritesheet
            {
                //this kind of implementation should give a performance boost in case of larger spritesheets as we don't have to load all the textures instantly
                //perhaps we could make textures a vector of pointers instead though, in order to save memory
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
            std::string getName(unsigned int index)
            {
                std::string out_ans;
                try
                {
                    out_ans = names.at(index); //the at function throws an out_of_range exception in case of too high index
                }
                catch(...)
                {
                    printf("accessing name out of bounds, attempting to get %u, max index is %u\n", index, (unsigned int)(names.size()) - 1u);
                    out_ans = getName(error_tile_index);
                }
                return out_ans;
            }
            bool inDictionary(std::string name)
            {
                return dictionary.find(hashFunc(name, hash_bases[hash_base_index], 1000000007)) != dictionary.end();
            }
            void setErrorTileIndex(unsigned int index)
            {
                //preventing setting invalid error_tile_index
                if(index >= sprite_count)
                {
                    printf("%u is not a valid tile index\n", index);
                    return;
                }
                error_tile_index = index;
            }
    };
}
#endif
