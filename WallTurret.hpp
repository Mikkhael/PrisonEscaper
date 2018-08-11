#ifndef TURRET_HPP_INCLUDED
#define TURRET_HPP_INCLUDED

#include "TextureManager.hpp"
#include "Object.hpp"


class WallTurretBase : public SpriteActor, public ActorCollection<WallTurretBase>
{
public:
    
};



/*
class WallTurret : public MultipleSpritesActor, public ActorCollection<WallTurret>
{
public:
    
    enum BaseDirection{Left=0, Up=1, Right=2, Down=3};
private:
    
    static const int gunXOffset = 2;
    
    sf::Sprite baseSprite;
    sf::Sprite gunSprite;
    
    BaseDirection baseDirection;
    Vector2d directionVector;
    
public:
    
    
    
    void setDirection(BaseDirection bd)
    {
        if(bd == baseDirection)
        {
            return;
        }
        baseDirection = bd;
        switch(bd)
        {
        case Left:
            {
                directionVector = Vector2d(1, 0);
                setRotation(0);
                break;
            }
        case Right:
            {
                directionVector = Vector2d(-1, 0);
                setRotation(180);
                break;
            }
        case Up:
            {
                directionVector = Vector2d(0, 1);
                setRotation(90);
                break;
            }
        case Down:
            {
                directionVector = Vector2d(0, -1);
                setRotation(-90);
                break;
            }
        }
    }

    WallTurret(BaseDirection baseDirection_, Vector2d position)
        :baseSprite(textureManager.get(WallTurretSprite::path), WallTurretSprite::Base::rect),
         gunSprite(textureManager.get(WallTurretSprite::path), WallTurretSprite::Gun::rect)
    {
        
        appendSprite(baseSprite);
        appendSprite(gunSprite);
        
        setPosition(position);
        baseSprite.setOrigin(0, WallTurretSprite::Base::rect.height/2);
        gunSprite.setOrigin(0, WallTurretSprite::Gun::rect.height/2);
        setDirection(baseDirection_);
        
    }
};
*/

#endif // TURRET_HPP_INCLUDED
