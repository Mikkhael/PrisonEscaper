#ifndef LIGHTEMITTER_HPP_INCLUDED
#define LIGHTEMITTER_HPP_INCLUDED
#include "Object.hpp"
#include "PLatform.hpp"
#include <vector>

class LightEmitter : public SimpleTransformable, public Collection<LightEmitter>
{
protected:
    static sf::RenderTexture* defaultTempLightmapTexture;
    static sf::RenderTexture* defaultLightmapTexture;
    
    virtual void emitLightOn(sf::RenderTexture* tempLightmapTexture = defaultTempLightmapTexture) = 0;
    
    
public:
    
    static void generateLightMap(sf::RenderTexture* lightmapTexture = defaultLightmapTexture, sf::RenderTexture* tempLightmapTexture = defaultTempLightmapTexture)
    {
        if(!lightmapTexture || !tempLightmapTexture)
        {
            return;
        }
        lightmapTexture->clear();
        iterate([&](LightEmitter& le)
        {
            le.emitLightOn(tempLightmapTexture);
            tempLightmapTexture->display();
            lightmapTexture->draw(sf::Sprite(tempLightmapTexture->getTexture()));
            return false;
        });
    }
    
    static void applyLightMap(sf::RenderTarget& target, sf::RenderTexture* lightmapTexture = defaultLightmapTexture)
    {
        if(!lightmapTexture)
        {
            return;
        }
        target.draw(sf::Sprite(lightmapTexture->getTexture()), sf::RenderStates(sf::BlendMultiply));
    }
    
    static void generateAndApplyLightMap(sf::RenderTarget& target, sf::RenderTexture* lightmapTexture = defaultLightmapTexture, sf::RenderTexture* tempLightmapTexture = defaultTempLightmapTexture)
    {
        if(!lightmapTexture || !tempLightmapTexture)
        {
            return;
        }
        generateLightMap(lightmapTexture, tempLightmapTexture);
        lightmapTexture->display();
        applyLightMap(target, lightmapTexture);
    }
    
    static void createDefaultLightmapTextures(sf::Vector2u size)
    {
        if(defaultLightmapTexture)
        {
            delete defaultLightmapTexture;
        }
        defaultLightmapTexture = new sf::RenderTexture;
        defaultLightmapTexture->create(size.x, size.y);
        
        if(defaultTempLightmapTexture)
        {
            delete defaultTempLightmapTexture;
        }
        defaultTempLightmapTexture = new sf::RenderTexture;
        defaultTempLightmapTexture->create(size.x, size.y);
    }
    
    virtual ~LightEmitter()
    {
        if(defaultLightmapTexture)
        {
            delete defaultLightmapTexture;
        }
        
        if(defaultTempLightmapTexture)
        {
            delete defaultTempLightmapTexture;
        }
    }
    
};


sf::RenderTexture* LightEmitter::defaultTempLightmapTexture = nullptr;
sf::RenderTexture* LightEmitter::defaultLightmapTexture = nullptr;

class PointLightEmitter : public LightEmitter
{
    sf::CircleShape shape;
    
    void mapPlatformsShadows(sf::RenderTexture* tempLightmapTexture)
    {
        sf::VertexArray shadow(sf::PrimitiveType::TriangleStrip, 5);
        
        shadow[0].color = sf::Color::Black;
        shadow[1].color = sf::Color::Black;
        shadow[2].color = sf::Color::Black;
        shadow[3].color = sf::Color::Black;
        shadow[4].color = sf::Color::Black;
        
        double radius2_2 = getRadius() * getRadius() * 4;
        
        for(Platform& platform : platforms)
        {
            shadow[0].position = platform.collider.position;
            shadow[2].position = platform.collider.getEnd();
            
            Vector2d sourceToPoint1 = platform.collider.position - getPosition();
            Vector2d sourceToPoint2 = platform.collider.getEnd() - getPosition();
            Vector2d sourceToPoint3 = (platform.collider.getCenter() - getPosition());
            
            shadow[1].position = getPosition() + (sourceToPoint1.magnatudeSquared() <  radius2_2 ? sourceToPoint1.resize(getRadius()*2) : sourceToPoint1);
            shadow[4].position = getPosition() + (sourceToPoint2.magnatudeSquared() <  radius2_2 ? sourceToPoint2.resize(getRadius()*2) : sourceToPoint2);
            
            shadow[3].position = getPosition() + (sourceToPoint3.magnatudeSquared() <  radius2_2 ? sourceToPoint3.resize(getRadius()*2) : sourceToPoint3);
            
            tempLightmapTexture->draw(shadow);
        }
    }
protected:    
    
    
    
    virtual void emitLightOn(sf::RenderTexture* tempLightmapTexture = defaultTempLightmapTexture)
    {
        if(!tempLightmapTexture)
        {
            return;
        }
        shape.setPosition(getPosition());
        shape.setRotation(getRotation());
        shape.setScale(getScale());
        tempLightmapTexture->clear();
        tempLightmapTexture->draw(shape);
        
        mapPlatformsShadows(tempLightmapTexture);
    }
public:
    
    double getRadius()
    {
        return shape.getRadius();
    }
    void setRadius(double radius)
    {
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
    }
    
    PointLightEmitter(double radius)
    {
        setRadius(radius);
        shape.setFillColor(sf::Color::White);
    }
    
    virtual ~PointLightEmitter(){};
};

#endif // LIGHTEMITTER_HPP_INCLUDED
