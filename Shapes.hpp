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
	
	Rect(const Vector2<T>& pos, const Vector2<T>& s)
		: position(pos), size(s)
	{}
	
	template<class T2>
	Rect(const sf::Rect<T2>& r)
		: position(r.left, r.top), size(r.width, r.height)
	{}
	
	template<class T2>
	operator sf::Rect<T2>()
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
	
	Rect<T> rotate(int rotations) const
	{
	    rotations %= 4;
	    if(rotations < 0)
        {
            rotations += 4;
        }
        
        Rect<T> rect = Rect<T>(position, size);
        T temp;
        while(rotations-- > 0)
        {
            temp = rect.position.x;
            rect.position.x = -(rect.position.y + rect.size.y);
            rect.position.y = temp;
            
            rect.size.swapSelf();
        }
        
        if(tak)
            std::cout << "R: " << rect.position << " " << rect.size << std::endl;
        
        return rect;
	}
	
};


template<class T>
class Line
{
public:
    
    Vector2<T> point1;
    Vector2<T> point2;
    
    Line<T> move(const Vector2<T>& v) const
    {
        return Line<T>(point1 + v, point2 + v);
    }
    Line<T>& moveSelf(Vector2<T> v)
    {
        point1 += v;
        point2 += v;
        return *this;
    }
    Line<T> scale(const Vector2<T>& v) const
    {
        return Line<T>(point1 * v, point2 * v);
    }
    Line<T>& scaleSelf(Vector2<T> v)
    {
        point1 *= v;
        point2 *= v;
        return *this;
    }
    Line<T> rotate(double angle) const
    {
        return Line<T>(point1.rotate(angle), point2.rotate(angle));
    }
    Line<T>& rotateSelf(double angle)
    {
        point1.rotateSelf(angle);
        point2.rotateSelf(angle);
        return *this;
    }    
    
    Vector2<T> toVector() const
    {
        return point2 - point1;
    }
    long double length() const
    {
        return toVector().magnatude();
    }
    long double lengthSquared() const
    {
        return toVector().magnatudeSquared();
    }
    
    
    Line(const Vector2<T>& vector)
        : point1(Vectors::null), point2(vector)
    {}
    
    Line(const Vector2<T>& p1, const Vector2<T>& p2)
        : point1(p1), point2(p2)
    {}
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
	
	Vector2<T> getCenter() const
	{
		if(isVertical)
		{
			return Vector2<T>(position.x, position.y + length*0.5);
		}
		return Vector2<T>(position.x + length*0.5, position.y);
	}
	
	Vector2<T> getVector() const
	{
		if(isVertical)
		{
			return Vector2<T>(0, length);
		}
		return Vector2<T>(length, 0);
	}
	
	T getStartValue() const
	{
	    if(isVertical)
            return position.y;
        return position.x;
	}
	void setStartValue(T val)
	{
        if(isVertical)
        {
            length -= val - position.y;
            position.y = val;
        }
        else
        {
            length -= val - position.x;
            position.x = val;
        }
	}
	T getEndValue() const
	{
	    if(isVertical)
            return position.y + length;
        return position.x + length;
	}
	void setEndValue(T val)
	{
        if(isVertical)
            length = val - position.y;
        else
            length = val - position.x;
	}
	
	T getOffset() const
	{
	    if(isVertical)
            return position.x;
        return position.y;
	}
	void setOffset(T val)
	{
	    if(isVertical)
            position.x = val;
        else
            position.y = val;
	}
	
	Line<T> toLine() const
	{
	    return Line<T>(position, getEnd());
	}
	
	Rect<T> toRect() const
	{
		if(isVertical)
			return Rect<T>(position, {1, length});
		return Rect<T>(position, {length, 1});
	}
};

#endif // SHAPES_HPP_INCLUDED
