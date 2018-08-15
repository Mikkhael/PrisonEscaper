#ifndef TURRET_HPP_INCLUDED
#define TURRET_HPP_INCLUDED

#include "TextureManager.hpp"
#include "Object.hpp"


class WallTurret : public SpriteActor, public ActorCollection<WallTurret>
{
protected:
    void updateCollider()
    {
        tak = true;
        SpriteActor::updateCollider();
        tak = false;
    }
public:
    
    enum BaseDirection{Left=0, Up=1, Right=2, Down=3};
private:
    
    
    
    
    class Gun : public SpriteActor
    {
    public:
        Gun()
            : SpriteActor(textureManager.get(WallTurretSprite::path), WallTurretSprite::Gun::rect)
        {
            sprite.setOrigin(0, WallTurretSprite::Gun::rect.height/2);
        }
        
    } gun;
    
    
    BaseDirection baseDirection;
    Vector2d directionVector = Vectors::null;
    double gunOffset = 3;
    
    
    void updateDirectionVectorAndRotation()
    {
        if(baseDirection == Left)
        {
            directionVector = Vector2d(1, 0);
            setRotation(0);
        }
        else if(baseDirection == Right)
        {
            directionVector = Vector2d(-1, 0);
            setRotation(180);
        }
        else if(baseDirection == Up)
        {
            directionVector = Vector2d(0, 1);
            setRotation(90);
        }
        else if(baseDirection == Down)
        {
            directionVector = Vector2d(0, -1);
            setRotation(-90);
        }
    }
    
    void updateGunTransform()
    {
        Vector2d offsetVector = gunOffset * directionVector * getScale();
        gun.setTransform(getTransform());
        gun.rotate(gunAngle);
        gun.move(offsetVector);
    }
    
    
    double minAngle = -45;
    double maxAngle = 45;
    double waitTime = 1;
    double switchTime = 2;
    
    bool gunIsRotatingClockwise = true;
    bool gunIsRotating = true;
    
    double gunAngle = 0;
    
    double gunWaitCounter = 0;
    void updateGunAngle(double deltaTime)
    {
        bool toSwitch = false;
        if(gunIsRotating)
        {
            gunAngle += deltaTime * (maxAngle - minAngle) / switchTime * (gunIsRotatingClockwise ? 1 : -1);
            if(gunAngle > maxAngle)
            {
                gunAngle = maxAngle;
                toSwitch = true;
            }
            else if(gunAngle < minAngle)
            {
                gunAngle = minAngle;
                toSwitch = true;
            }
            
            if(toSwitch)
            {
                gunIsRotatingClockwise = !gunIsRotatingClockwise;
                gunIsRotating = false;
            }
        }
        else
        {
            gunWaitCounter += deltaTime;
            if(gunWaitCounter > waitTime)
            {
                gunWaitCounter = 0;
                gunIsRotating = true;
            }
        }
    }
    
public:
    
    void update(double deltaTime)
    {
        updateGunAngle(deltaTime);
        updateGunTransform();
    }
    
    virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const
    {
        SpriteActor::draw(target, states);
        gun.draw(target, states);
    }
    
    void setBaseDirection(BaseDirection bd)
    {
        if(baseDirection == bd)
        {
            return;
        }
        baseDirection = bd;
        updateDirectionVectorAndRotation();
    }
    
    WallTurret(BaseDirection baseDirection_, const Vector2d& position)
        : SpriteActor(textureManager.get(WallTurretSprite::path), WallTurretSprite::Base::rect),
          baseDirection(baseDirection_)
    {
        setPosition(position);
        setScale({2,2});
        sprite.setOrigin(0, WallTurretSprite::Base::rect.height / 2);
        setCollider(Rect<double>(Vector2d(0, -WallTurretSprite::Base::rect.height / 2), Vector2d(WallTurretSprite::Base::rect.width, WallTurretSprite::Base::rect.height)));
        updateDirectionVectorAndRotation();
    }
    
    
    
    
    
    
    
    
    
    
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
