#ifndef TEXTURESINFO_HPP_INCLUDED
#define TEXTURESINFO_HPP_INCLUDED


namespace PlayerSprite
{
    const std::string path       = "assets/creatures/player.bmp";
    const unsigned int width     = 16;
    const unsigned int height    = 16;
}

namespace CannonballSprite
{
    const std::string path       = "assets/objects/cannonball.bmp";
    const sf::IntRect rect(0,0,5,5);
}

namespace WallTurretSprite
{
    const std::string path       = "assets/objects/turret.bmp";
    
    namespace Offset
    {
        const sf::Vector2i blue(0,0);
        const sf::Vector2i red(0,12);
    }
    
    namespace Base
    {
        const sf::IntRect rect(0,0,5,11);
    }
    namespace Gun
    {
        const sf::IntRect rect(6,3,10,5);
    }
    
}

#endif // TEXTURESINFO_HPP_INCLUDED
