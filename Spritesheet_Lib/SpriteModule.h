#include "SpriteModule.cpp"
#ifndef SPRITEMODULE_H
#define SPRITEMODULE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
namespace m2d
{
    unsigned long long hashFunc(std::string to_hash, unsigned long long hash_base, unsigned long long hash_mod);
    class SpriteSheet
    {
        private:
            Vector2u sprite_size;
            Image sheet;
            unsigned int sprite_count;
            std::vector<Texture> textures;
            std::vector<bool> loaded;
            unsigned int hash_base_index;
            std::vector<unsigned int> hash_bases;
            std::map<unsigned long long, int> dictionary;
            std::vector<std::string> names;
            unsigned int error_tile_index;
            void loadTexture(unsigned int index);
            void initDictionary(std::string dictionary_file_name);
            void objectInit(std::string file_name, sf::Vector2u sprsize);
        public:
            SpriteSheet();
            SpriteSheet(std::string file_name, sf::Vector2u sprsize);
            SpriteSheet(std::string image_file_name, sf::Vector2u _sprsize, std::string dictionary_file_name);
            sf::Vector2u getSize(); //returns the size of the initial image
            sf::Vector2u getSprsize(); //returns sprite size
            unsigned int getTileIndex(std::string name); //returns a numerical index of a tile with the chosen name
            sf::Texture& getTexture(unsigned int index); //returns the tile texture of the chosen index
            sf::Texture& getTexture(std::string name); //returns the tile texture of the chosen name
            std::string getName(unsigned int index); //returns the name of the tile with the chosen index
            bool inDictionary(std::string name); //checks whether a name is already inside the dictionary
            void setErrorTileIndex(unsigned int index); //sets the tile which will be returned in case of tile fetching errors, default is 0
    };
}
#endif
