#ifndef VECTORS_HPP_INCLUDED
#define VECTORS_HPP_INCLUDED

#include <iostream>
#include <cmath>

constexpr double	M_PI 		= 3.14159265358979323846;
constexpr double  	M_PI_2		= 1.57079632679489661923;
constexpr double  	M_PI_180	= 0.01745329251994329577;

inline double toRadians(double degrees)
{
	return degrees*M_PI_180;
}

template<typename T>
class Vector2
{
public:
    
    T x;
    T y;
        
    template<typename T_>
    Vector2<T> operator+(const Vector2<T_>& v) const
    {
        return Vector2<T>(x + v.x, y + v.y);
    }
    template<typename T_>
    Vector2<T> operator-(const Vector2<T_>& v) const
    {
        return Vector2<T>(x - v.x, y - v.y);
    }
    Vector2<T> operator*(T s) const
    {
        return Vector2<T>(x * s, y * s);
    }
    Vector2<T> operator/(T s) const
    {
        return Vector2<T>(x / s, y / s);
    }
    template<typename T_>
    Vector2<T> operator*(const Vector2<T_>& v) const
    {
        return Vector2<T>(x * v.x, y * v.y);
    }
    template<typename T_>
    Vector2<T> operator/(const Vector2<T_>& v) const
    {
        return Vector2<T>(x / v.x, y / v.y);
    }
    
    template<typename T_>
    Vector2<T>& operator+=(const Vector2<T_>& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    template<typename T_>
    Vector2<T>& operator-=(const Vector2<T_>& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vector2<T>& operator*=(T s)
    {
        x *= s;
        y *= s;
        return *this;
    }
    Vector2<T>& operator/=(T s)
    {
        x /= s;
        y /= s;
        return *this;
    }
    template<typename T_>
    Vector2<T>& operator*=(const Vector2<T_>& v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }
    template<typename T_>
    Vector2<T>& operator/=(const Vector2<T_>& v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }
    
    template<typename T_>
    T dot(const Vector2<T_>& v) const
    {
        return x*v.x + y*v.y;
    }
    template<typename T_>
    T cross(const Vector2<T_>& v) const
    {
        return x*v.y - y*v.x;
    }
    
    Vector2 sort() const
    {
    	if(x < y)
		{
			return Vector2(x, y);
		}
		return Vector2(y, x);
    }
    
    Vector2& sortSelf()
    {
    	if(x > y)
		{
			T temp = x;
			x = y;
			y = temp;
		}
		return *this;
    }
    
    template<typename T_>
    Vector2<T>& merge(Vector2<T_> v) const
    {
        return Vector2<T>(std::max(x, v.x), std::min(y, v.y));
    }
    template<typename T_>
    Vector2<T>& mergeSelf(Vector2<T_> v) const
    {
        x = std::max(x, v.x);
        y = std::min(y, v.y);
        return *this;
    }
    
    template<typename T_>
    Vector2<T> resize(T_ size) const
    {
    	return normalize()*size;
    }
    template<typename T_>
    Vector2<T>& resizeSelf(T_ size)
    {
    	normalizeSelf();
    	*this *= size;
    	return *this;
    }
    
    Vector2<T> swap() const
    {
        return Vector2<T>(y, x);
    }
    
    Vector2<T>& swapSelf()
    {
        T temp = x;
        x = y;
        y = temp;
        return *this;
    }
    
    Vector2<T> rotate90(bool clockwise = true) const
    {
        return Vector2<T>(-y, x);
    }
    
    Vector2<T>& rotateSelf90(bool clockwise = true)
    {
        if(clockwise)
        {
            T temp = x;
            x = -y;
            y = temp;
        }
        else
        {
            T temp = x;
            x = y;
            y = -temp;
        }
        return *this;
    }
    
    long double magnatudeSquared() const;
    long double magnatude() const;
    Vector2<double> normalize() const;
    Vector2<T>& normalizeSelf();
    Vector2<T> rotate(double angle) const;
    Vector2<T>& rotateSelf(double angle);
    
    
    
    Vector2(){}
    Vector2(T _x, T _y)
        : x(_x), y(_y)
    {}
        
    template<typename _T>
    Vector2(const sf::Vector2<_T>& v)
        : x(v.x), y(v.y)
    {}
    
    
    template<typename _T>
    operator sf::Vector2<_T>() const
    {
        return sf::Vector2<_T>(x, y);
    }
    
	template<typename _T>
    operator Vector2<_T>() const
    {
        return Vector2<_T>(x, y);
    }
    
    bool isNull()
    {
    	return x == 0 && y == 0;
    }
    
    
    void SHOW()
    {
        std::cout<<"("<<x<<" , "<<y<<")"<<std::endl;
    }
    
};

template<typename T>
long double Vector2<T>::magnatudeSquared() const
{
    return x*x+y*y;
}

template<typename T>
long double Vector2<T>::magnatude() const
{
    return std::sqrt(x*x+y*y);
}

template<typename T>
Vector2<double> Vector2<T>::normalize() const
{
    auto mag = magnatude();
    if(mag != 0)
        return Vector2<double>(x/mag, y/mag);
    return Vector2<double>(0, 0);
}

template<typename T>
Vector2<T>& Vector2<T>::normalizeSelf()
{
    auto mag = magnatude();
    if(mag != 0)
    {
        x /= mag;
        y /= mag;        
    }
    return *this;    
}

template<typename T>
Vector2<T> Vector2<T>::rotate(double angle) const
{
    auto c = std::cos(toRadians(angle));
    auto s = std::sin(toRadians(angle));
    return Vector2<T>(c*x-s*y, c*y+s*x);
}

template<typename T>
Vector2<T>& Vector2<T>::rotateSelf(double angle)
{
    auto c = std::cos(toRadians(angle));
    auto s = std::sin(toRadians(angle));
    auto tx = x;
    x = c*x-s*y;
    y = c*y+s*tx;
    return *this;
}

template<typename T>
Vector2<T> operator*(T s, const Vector2<T>& v)
{
    return Vector2<T>(v.x * s, v.y * s);
}
template<typename T>
Vector2<T> operator/(T s, const Vector2<T>& v)
{
    return Vector2<T>(s / v.x, s / v.y);
}

using Vector2i      = Vector2<int>;
using Vector2u      = Vector2<unsigned int>;
using Vector2ull    = Vector2<unsigned long long>;
using Vector2d      = Vector2<double>;
using Vector2f      = Vector2<float>;
using Vector2ld     = Vector2<long double>;

namespace Vectors
{
    const Vector2d foreward(1,0);
    const Vector2d backward(-1,0);
    const Vector2d up(0,1);
    const Vector2d down(0,-1);
    const Vector2d units(1,1);
    const Vector2d null(0,0);
}

struct Transform
{
	Vector2<double> position;
	double rotation;
	Vector2<double> scale;
	
	Transform(const Vector2d& position_ = Vectors::null, double rotation_ = 0, const Vector2d& scale_ = Vectors::units)
        : position(position_), rotation(rotation_), scale(scale_)
	{
	}
};

class Transformable
{
public:
	virtual void setPosition (const Vector2d& 	position)	= 0;
	virtual void setScale	 (const Vector2d&	scale) 		= 0;
	virtual void setRotation (double 			rotation) 	= 0;
	
	virtual Vector2d getPosition() 	const  	= 0;
	virtual Vector2d getScale()		const	= 0;
	virtual double	 getRotation()	const	= 0;
	
	void move(const Vector2d& position)
	{
		setPosition(getPosition() + position);
	}
	void scale(const Vector2d& scale)
	{
		setScale(getScale() * scale);
	}
	void rotate(double rotation)
	{
		setRotation(getRotation() + rotation);
	}
	
	void setTransform(const Transformable& t)
	{
	    setPosition(t.getPosition());
	    setRotation(t.getRotation());
	    setScale(t.getScale());
	}
	void setTransform(const Transform& t)
	{
	    setPosition(t.position);
	    setRotation(t.rotation);
	    setScale(t.scale);
	}
	void setTransform(const Vector2d& position, double rotation, const Vector2d& scale)
	{
	    setPosition(position);
	    setRotation(rotation);
	    setScale(scale);
	}
	
	void applyTransformTo(sf::Transformable& t) const
	{
	    t.setPosition(getPosition());
	    t.setRotation(getRotation());
	    t.setScale(getScale());
	}
	
	Transform getTransform() const
	{
	    return Transform(getPosition(), getRotation(), getScale());
	}
	
	virtual ~Transformable(){};
};

class SimpleTransformable : public Transformable
{
    Transform transform;
public:
    
    virtual void setPosition (const Vector2d& 	position)
	{
		transform.position = position;
	}
	virtual void setScale	 (const Vector2d&	scale)
	{
		transform.scale = scale;
	}
	virtual void setRotation (double 			rotation)
	{
		transform.rotation = rotation;
	}
	
	virtual Vector2d getPosition() 	const
	{
		return transform.position;
	}
	virtual Vector2d getScale() 	const
	{
		return transform.scale;
	}
	virtual double	 getRotation()	const
	{
		return transform.rotation;
	}
	
	virtual ~SimpleTransformable(){};
};

template<class T>
std::ostream& operator<<(std::ostream& cout, const Vector2<T>& vector)
{
    cout<<vector.x<<" , "<<vector.y;
    return cout; 
}


#endif // VECTORS_HPP_INCLUDED
