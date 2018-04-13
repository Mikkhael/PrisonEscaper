#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED


#include "Vectors.hpp"
#include "TextureManager.hpp"
#include "Animations.hpp"
#include "Colisions.hpp"
#include "Shapes.hpp"

/*
void flip(sf::Sprite& s)
{
    auto r = s.getTextureRect();
    s.setTextureRect(sf::IntRect(r.left + r.width, r.top, -r.width, r.height));
}

Vector2d getMouse(const sf::RenderWindow& rt)
{
    auto m = sf::Mouse::getPosition(rt);
    return Vector2d(m.x, m.y);
}

Vector2d getRealMouse(const sf::RenderWindow& rt)
{
    auto m = sf::Mouse::getPosition(rt);
    return Vector2d(m.x * rt.getView().getSize().x / rt.getSize().x, m.y * rt.getView().getSize().y / rt.getSize().y);
}

Vector2d mouseRealPosition(0,0);
class Object
{
public:
    sf::Sprite sprite;
    std::string name;

    Animation* animation = nullptr;
    void setAnimation(const AnimationPreset& ap, const AnimationState state = Once)
        if(animation == nullptr)
        {
            animation = new Animation(ap);
        }
        else
        {
            animation->setPreset(ap);
        }
        animation->state = state;
    }

    Collider* collider = nullptr;
    template<class T>
    void setCollider(T c)
    {
        if(collider == nullptr)
        {
            collider = new ShapeCollider<T>(c);
        }
        else
        {
            delete collider;
            collider = new ShapeCollider<T>(c);
        }
    }
    void updateCollider()
    {
        collider->updateCollider(sprite);
    }
    bool testCollision(Object& o, bool inclusive = true)
    {
        if(collider == nullptr || o.collider == nullptr)
        {
            return false;
        }
        updateCollider();
        o.updateCollider();
        return collider->test(o.collider, inclusive);
    }
    template<class T>
    bool testCollision(const T& o, bool inclusive = true)
    {
        updateCollider();
        return collider->test(o, inclusive);
    }


    void base_update(double deltaTime)
    {
        if(animation != nullptr)
        {
            animation->update(deltaTime);
        }

        update(deltaTime);
    }

    virtual void update(double deltaTime)
    {

        return;
    }
    virtual void draw(sf::RenderTarget& rt)
    {
        if(animation != nullptr)
        {
            sprite.setTextureRect(animation->get());
        }
        rt.draw(sprite);
    }

    void setSize(const sf::IntRect& rect)
    {
        sprite.setTextureRect(rect);
    }
    
    Object()
        : name("")
    {}
    
    Object(const std::string& texture, const std::string& n = "")
        : name(n)
    {
        sprite.setTexture(textureManager.get(texture));
    }
    
    Object(const std::string& texture, const AnimationPreset& ap, const std::string& n = "", AnimationState state=Loop)
        : name(n)
    {
        sprite.setTexture(textureManager.get(texture));
        animation = new Animation(ap);
        animation->state = state;
    }
    
    Object(const std::string& texture, const sf::IntRect& rect, const std::string& n = "")
        : name(n)
    {
        sprite.setTexture(textureManager.get(texture));
        sprite.setTextureRect(rect);
    }
    
    virtual ~Object()
    {
        if(animation != nullptr)
            delete animation;
    }

};
*/

class Actor
{
public:
	Transform transform;
	Collider* collider;
	
	
	
	virtual void update(double deltaTime)
	{
		return;
	}
	
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) = 0;
	
	
	Collision::Result testCollision(const Collider& collider_) const
	{
		if(collider)
		{
			return collider->test(collider_);
		}
		return Collision::Result(false);
	}
	Collision::Result testCollision(const Actor& actor_) const
	{
		if(collider && actor_.collider)
		{
			return collider->test(*actor_.collider);
		}
		return Collision::Result(false);
	}
	
	template<class T>
    void setCollider(const T& shape)
    {
        if(collider)
        {
            delete collider;
            collider = new ShapeCollider<T>(shape);
        }
        else
        {
            collider = new ShapeCollider<T>(shape);
        }
    }
    
    
    
	
	Actor()
		: collider(nullptr)
	{}
	
	virtual ~Actor()
	{
		if(collider)
		{
			delete collider;
		}
	}
	
};


class SpriteActor : public Actor
{
	
	void updateTransform()
	{
		sprite.setPosition(transform.position);
		sprite.setScale(transform.scale);
		sprite.setRotation(transform.rotation);
	}
	
public:
	
	sf::Sprite sprite;
	
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default)
	{
		updateTransform();
		target.draw(sprite, states);
	}
	
	
	
	SpriteActor(const sf::Texture& texture, const sf::IntRect& rect)
		: sprite(texture)
	{
		sprite.setTextureRect(rect);
	}
	
	virtual ~SpriteActor(){};
	
};

class AnimatedSpriteActor : public Actor
{
	
	void updateTransform()
	{
		sprite.setPosition(transform.position);
		sprite.setScale(transform.scale);
		sprite.setRotation(transform.rotation);
	}
	
public:
	
	AnimatedSprite sprite;
	
	
	virtual void update(double deltaTime)
	{
		sprite.updateFrame(deltaTime);
	}
	
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default)
	{
		updateTransform();
		target.draw(sprite, states);
	}
	
	
	
	AnimatedSpriteActor(const AnimatedSpritePreset& preset)
		: sprite(preset)
	{
		
	}
	
	virtual ~AnimatedSpriteActor(){};
};


#endif // OBJECT_HPP_INCLUDED
