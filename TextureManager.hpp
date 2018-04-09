#ifndef TEXTUREMANAGER_HPP_INCLUDED
#define TEXTUREMANAGER_HPP_INCLUDED
#include <map>

class TextureManager
{
public:
	
    std::map<const std::string, sf::Texture*> textures;
        
    void load(const std::string& path, bool repeat = true)
    {
        sf::Image img;
        img.loadFromFile(path);
        img.createMaskFromColor(sf::Color::Magenta);
        sf::Texture* T = new sf::Texture;
        T->loadFromImage(img);
        if(repeat)
        {
            T->setRepeated(true);
        }
        textures[path] = T;
    }
    
    
    sf::Texture& get(const std::string& path, bool repeat = true)
    {
        if(textures.find(path) == textures.end())
        {
            load(path, repeat);
        }
        return *textures[path];
    }
    
};
TextureManager textureManager;

#endif // TEXTUREMANAGER_HPP_INCLUDED
