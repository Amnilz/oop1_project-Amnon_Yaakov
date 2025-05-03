#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class ResourceManager 
{
public:
   
    static ResourceManager& instance();
    sf::Texture& getTexture(const std::string& name);
    sf::SoundBuffer& getSound(const std::string& name);

    sf::Font& getFont(const std::string& filename);

private:
   
    ResourceManager();
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::SoundBuffer> m_sounds;
    std::map<std::string, sf::Font> m_fonts;
};
