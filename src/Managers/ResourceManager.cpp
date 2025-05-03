#include "Managers/ResourceManager.h"
#include <stdexcept> 
#include <iostream>

ResourceManager::ResourceManager(){}

ResourceManager& ResourceManager::instance()
{
    static ResourceManager inst;
    return inst;
}


sf::Texture& ResourceManager::getTexture(const std::string& filename)
{
    auto it = m_textures.find(filename);
    if (it != m_textures.end())
    {
        
        return it->second;
    }
    sf::Texture texture;
    if (!texture.loadFromFile(filename))
    {
        std::cerr << "Could not open texture: " << filename << std::endl;
     
        exit(EXIT_FAILURE);
    }
    m_textures[filename] = std::move(texture);
    return m_textures[filename];
}
sf::SoundBuffer& ResourceManager::getSound(const std::string& filename)
{
    
    auto it = m_sounds.find(filename);
    if (it != m_sounds.end())
    {
        return it->second;
    }
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(filename))
    {
        std::cerr << "Could not open sound file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    m_sounds[filename] = std::move(soundBuffer);
    return m_sounds[filename];
}
sf::Font& ResourceManager::getFont(const std::string& filename)
{
    auto it = m_fonts.find(filename);
    if (it != m_fonts.end())
    {
        return it->second;
    }
    sf::Font font;
    if (!font.loadFromFile(filename))
    {
        std::cerr << "Could not open font file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    m_fonts[filename] = std::move(font);
    return m_fonts[filename];
}



