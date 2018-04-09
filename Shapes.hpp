#ifndef SHAPES_HPP_INCLUDED
#define SHAPES_HPP_INCLUDED

#include "Vectors.hpp"

template<class T>
class Circle
{
public:
    Vector2<T> position;
    T radius;

    Circle(const Vector2<T>& v, T r)
        : position(v), radius(r)
    {}
    
    Vector2<T> getRangeX() const
    {
    	return Vector2<T>(position.x - radius, position.x + radius);
    }
    Vector2<T> getRangeY() const
    {
    	return Vector2<T>(position.y - radius, position.y + radius);
    }
};

template<class T>
class Rect
{
public:
	Vector2<T> position;
	Vector2<T> size;
	
	Rect(const T left, const T top, const T width = 0, const T height = 0)
		: position(left, top), size(width, height)
	{}
	
	Rect(const Vector2<T>& pos, const Vector2<T> s)
		: position(pos), size(s)
	{}
	
	Rect(const sf::Rect<T>& r)
		: position(r.left, r.right), size(r.width, r.height)
	{}
	
	operator sf::Rect<T>()
	{
		return sf::Rect<T>(position.x, position.y, size.x, size.y);
	}
	
	Vector2<T> getUpperRight() const
	{
		return Vector2<T>(position.x + size.x, position.y);
	}
	Vector2<T> getUpperLeft() const
	{
		return Vector2<T>(position.x, position.y);
	}
	Vector2<T> getBottomLeft() const
	{
		return Vector2<T>(position.x, position.y + size.y);
	}
	Vector2<T> getBottomRight() const
	{
		return Vector2<T>(position.x + size.x, position.y + size.y);
	}
	
	Vector2<T> getVertical() const
	{
		return Vector2<T>(position.y, position.y + size.y);
	}
	Vector2<T> getHorizontal() const
	{
		return Vector2<T>(position.x, position.x + size.x);
	}
	
};


template <class T>
class SimpleSegment
{
public:
	
	Vector2<T> position;
	T length;
	bool isVertical;
	
	SimpleSegment(const Vector2<T>& position_, T length_, bool isVertical_)
		: position(position_), length(length_), isVertical(isVertical_)
	{}
	
	Vector2<T> getRange() const
	{
		if(isVertical)
		{
			return Vector2<T>(position.y, position.y + length);
		}
		return Vector2<T>(position.x, position.x + length);
	}
	
	Vector2<T> getEnd() const
	{
		if(isVertical)
		{
			return Vector2<T>(position.x, position.y + length);
		}
		return Vector2<T>(position.x + length, position.y);
	}
};

#endif // SHAPES_HPP_INCLUDED
