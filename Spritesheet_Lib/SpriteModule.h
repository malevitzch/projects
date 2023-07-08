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
            void loadTexture(unsigned int index);
            void initDictionary(std::string dictionary_file_name);
            void objectInit(std::string file_name, sf::Vector2u sprsize);
        public:
            SpriteSheet();
            SpriteSheet(std::string file_name, sf::Vector2u sprsize);
            SpriteSheet(std::string image_file_name, sf::Vector2u _sprsize, std::string dictionary_file_name);
            sf::Vector2u getSize();
            sf::Vector2u getSprsize();
            unsigned int getTileIndex(std::string name);
            sf::Texture& getTexture(unsigned int index);
            sf::Texture& getTexture(std::string name);
            bool inDictionary(std::string name);
    };
}
#endif
