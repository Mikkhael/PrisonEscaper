#ifndef COLISIONS_HPP_INCLUDED
#define COLISIONS_HPP_INCLUDED

#include "Shapes.hpp"

using eType = double;

namespace CollisionFast
{
	
	// Value in Range
    template<class A, class B>
    bool test(const Vector2<A>& seg, B point, eType e = 0.0)
    {
    	A mx = std::max(seg.x, seg.y);
    	A mn = std::min(seg.x, seg.y);
    	
    	if(e*2 >= std::abs(mx - mn))
			return false;
        
		return  std::min(seg.x+e, seg.y-e) <= point &&
				std::max(seg.x+e, seg.y-e) >= point;
    }
    template<class A, class B>
    bool test(B point, const Vector2<A>& seg, eType e = 0.0){return test(seg,point,e);}

	// Range intersection
    template<class A, class B>
    bool test(const Vector2<A>& a, const Vector2<B>& b, eType e = 0.0)
    {
        return  std::min(a.x, a.y) + e < std::max(b.x, b.y) &&
				std::max(a.x, a.y) - e > std::min(b.x, b.y);
    }
	
	// Point in Rect
    template<class A, class B>
    bool test(const Rect<A>& a, const Vector2<B>& b, eType eHorizontal = 0.0, eType eVertical = 0.0)
    {
        return  test(a.getHorizontal(), b.x, eHorizontal) &&
                test(a.getVertical(),   b.y, eVertical);
    }
    template<class A, class B>
    bool test(const Vector2<B>& b, const Rect<A>& a, eType eHorizontal = 0.0, eType eVertical = 0.0){return test(a, b, eHorizontal, eVertical);}

	// Rect intersection
    template<class A, class B>
    bool test(const Rect<A>& a, const Rect<B>& b, eType eHorizontal = 0.0, eType eVertical = 0.0)
    {
        return  test(a.getHorizontal(), b.getHorizontal(), eHorizontal) &&
                test(a.getVertical(),   b.getVertical(),   eVertical);
    }
	
	// Point in Circle
    template<class A, class B>
    bool test(const Circle<A>& a, const Vector2<B>& b, eType e = 0.0)
    {
		return (b-a.center).magnatudeSquared() <= (a.radius-e)*(a.radius-e);
    }
    template<class A, class B>
    bool test(const Vector2<B>& b, const Circle<A>& a, eType e = 0.0){return test(a, b, e);}

	// Circle intersection
    template<class A, class B>
    bool test(const Circle<A>& a, const Circle<B>& b, eType e = 0.0)
    {
		return (b.center-a.center).magnatudeSquared() <  (a.radius+b.radius-e)*(a.radius+b.radius-e);
    }

	// Circle and Rect
    template<class A, class B>
    bool test(const Circle<A>& a, const Rect<B>& b, eType eHorizontal = 0.0, eType eVertical = 0.0)
    {
    	eType eCorner = std::sqrt(eHorizontal*eHorizontal + eVertical*eVertical);
        return  (test(b.getHorizontal(), a.center.x, eHorizontal) && test(Vector2<B>(b.position.y-a.radius,  b.position.y+b.size.y+a.radius),  a.center.y, eVertical  )) ||
                (test(b.getVertical(),   a.center.y, eVertical  ) && test(Vector2<B>(b.position.x-a.radius,  b.position.x+b.size.x+a.radius),  a.center.x, eHorizontal)) ||
                test(a, b.getUpperLeft(),   eCorner) ||
                test(a, b.getUpperRight(),  eCorner) ||
                test(a, b.getBottomLeft(),  eCorner) ||
                test(a, b.getBottomRight(), eCorner);
    }
    template<class A, class B>
    bool test(const Rect<B>& b, const Circle<A>& a, eType eHorizontal = 0.0, eType eVertical = 0.0){return test(a, b, eHorizontal, eVertical);}

};

namespace Collision
{
	struct Result
	{
		double xPenetration = 0;
		double yPenetration = 0;
		
		
		operator bool() const
		{
			return value;
		}
		
		Result operator -()
		{
			return Result(value, -xPenetration, -yPenetration);
		}
		
		Vector2<double> getPenetrationVector() const
		{
			return Vector2<double>(xPenetration, yPenetration);
		}
		
		Result(bool v = false, double x = 0, double y = 0)
			: value(v), xPenetration(x), yPenetration(y)
		{}
		Result(double x, double y)
			: value(x != 0 || y != 0), xPenetration(x), yPenetration(y)
		{}
		
		static Result& getBetter(Result& r1, Result& r2)
		{
			if(!r1)
				return r2;
			if(!r2);
				return r1;
			if(r1.getPenetrationVector().magnatudeSquared() <  r2.getPenetrationVector().magnatudeSquared())
				return r1;
			return r2;
		}
		static Result& getWorse(Result& r1, Result& r2)
		{
			if(!r1)
				return r2;
			if(!r2);
				return r1;
			if(r1.getPenetrationVector().magnatudeSquared() >  r2.getPenetrationVector().magnatudeSquared())
				return r1;
			return r2;
		}
		
	private:
		bool value = false;
	};
	
	// Point - Range
	template <class T>
	Result test( const T value, const Vector2<T>& range)
	{
		T lower  = value - std::min(range.x, range.y);
		T higher = std::max(range.x, range.y) - value;
		
		return Result(lower >=0 && higher >= 0, lower<higher?-lower:higher, 0);
	}
	template <class T>
	Result test(const Vector2<T>& range, const T value){return -test(value, range);}
	
	// Range - Range
	template <class T>
	Result test(const Vector2<T>& range1, const Vector2<T>& range2)
	{
		Vector2<T> r1 = range1.sort();
		Vector2<T> r2 = range2.sort();
		
		T toLower  = r1.y - r2.x;
		T toHigher = r2.y - r1.x;
		
		return Result( toLower <=0 || toHigher <= 0, toLower<toHigher?toHigher:-toLower, 0);
	}
	
	// Point - SimpleSegment
	template <class T>
	Result test(const Vector2<T>& point, const SimpleSegment<T>& ssegment)
	{
		if(ssegment.isVertical)
		{
			return test(point.y, ssegment.getRange());
		}
		return test(point.x, ssegment.getRange());
	}
	template <class T>
	Result test( const SimpleSegment<T>& ssegment, const Vector2<T>& point){return -test(point, ssegment);}
	
	
	// Point - Rect
	template <class T>
	Result test(const Vector2<T>& point, const Rect<T>& rect)
	{
		Result horizontal 	= test(point.x, rect.getHorizontal());
		Result vertical 	= test(point.y, rect.getVertical());
		
		return Result(horizontal && vertical, horizontal.xPenetration, vertical.xPenetration);
	}
	template <class T>
	Result test( const Rect<T>& rect, const Vector2<T>& point){return -test(point, rect);}
	
	
	// SimpleSegment - Rect
	template <class T>
	Result test(const SimpleSegment<T>& ssegment, const Rect<T>& rect)
	{
		Result hResult;
		Result vResult;
		if(ssegment.isVertical)
		{
			hResult = test(ssegment.position.x, rect.getHorizontal());
			vResult = test(ssegment.getRange(), rect.getVertical());
		}
		else
		{
			hResult = test(ssegment.getRange(), rect.getHorizontal());
			vResult = test(ssegment.position.y, rect.getVertical());
		}
			
		if(hResult && vResult)
		{
			return Result(true, hResult.xPenetration, vResult.xPenetration);
		}
		return Result(false);
	}
	template <class T>
	Result test( const Rect<T>& rect, const SimpleSegment<T>& ssegment){return -test(ssegment, rect);}
	
	// SimpleSegment - SimpleSegment
	template <class T>
	Result test(const SimpleSegment<T>& ssegment1, const SimpleSegment<T>& ssegment2)
	{
		// TODO
		
		return Result(false);
	}
	
	// Rect - Rect
	template <class T>
	Result test(const Rect<T>& rect1, const Rect<T>& rect2)
	{
		Result horizontal 	= test(rect1.getHorizontal(), 	rect2.getHorizontal());
		Result vertical 	= test(rect2.getVertical(), 	rect2.getVertical());
		
		return Result(horizontal && vertical, horizontal.xPenetration, vertical.xPenetration);
	}
	
	// Point - Circle
	template <class T>
	Result test(const Vector2<T>& point, const Circle<T>& circle)
	{
		Vector2<T> penetration = point-circle.position;
		if(penetration.magnatudeSquared() <= circle.radius * circle.radius)
		{
			penetration.resizeSelf(circle.radius - penetration.magnatude());
			return Result(true, penetration.x, penetration.y);
		}
		
		return Result(false, 0, 0);
	}
	template <class T>
	Result test( const Circle<T>& circle, const Vector2<T>& point){return -test(point, circle);}
	
	// SimpleSegment - Circle
	template <class T>
	Result test(const SimpleSegment<T>& ssegment, const Circle<T>& circle)
	{
		if(ssegment.isVertical)
		{
			if(CollisionFast::test(circle.position.y, ssegment.getRange()))
			{
				return test(ssegment.position.x, circle.getRangeX());
			}
		}
		else
		{
			if(CollisionFast::test(circle.position.x, ssegment.getRange()))
			{
				return test(ssegment.position.y, circle.getRangeY());
			}
		}
		Result r1 = test(ssegment.position, circle);
		Result r2 = test(ssegment.getEnd(), circle);
		
		return Result::getWorse(r1, r2);
		
	}
	template <class T>
	Result test( const Circle<T>& circle, const SimpleSegment<T>& ssegment){return -test(ssegment, circle);}
	
	// Circle - Circle
	template <class T>
	Result test(const Circle<T>& circle1, const Circle<T>& circle2)
	{
		Vector2<T> penetration = circle1.position-circle2.position;
		T distance = circle1.radius+circle2.radius;
		if(penetration.magnatudeSquared() <= (distance)*(distance) )
		{
			penetration.resizeSelf(distance - penetration.magnatude());
			return Result(true, penetration.x, penetration.y);
		}
		
		return Result(false, 0, 0);
	}
	
	// Rect - Circle
	template <class T>
	Result test(const Rect<T>& rect, const Circle<T>& circle)
	{
		Result vResult(false);
		Result hResult(false);
		if(CollisionFast::test(rect.getHorizontal(), circle.position.x))
		{
			vResult = Collision::test(Vector2<T>(rect.position.y - circle.radius, rect.position.y + rect.size.y + circle.radius), circle.position.y);
		}
		if(CollisionFast::test(rect.getVertical(),   circle.position.y))
		{
			hResult = Collision::test(Vector2<T>(rect.position.x - circle.radius, rect.position.x + rect.size.x  + circle.radius), circle.position.x);
		}
		
		if(vResult)
		{
			if(hResult)
			{
				if(std::abs(vResult.xPenetration) < std::abs(hResult.xPenetration))
				{
					return Result(true, 0, vResult.xPenetration);
				}
				else
				{
					return Result(true, hResult.xPenetration, 0);
				}
			}
			else
			{
				return Result(true, 0, vResult.xPenetration);
			}
		}
		else if(hResult)
		{
			return Result(true, hResult.xPenetration, 0);
		}
		else
		{
			long double tempDist, newDist;
			Vector2<T> temp;
			
			
			tempDist 	= (rect.getUpperLeft() - circle.position).magnatudeSquared();
			temp 		= rect.getUpperLeft();
			
			newDist = (rect.getUpperRight() - circle.position).magnatudeSquared();
			if(tempDist > newDist)
			{
				temp 		= rect.getUpperRight();
				tempDist	= newDist;
			}
			newDist = (rect.getBottomLeft() - circle.position).magnatudeSquared();
			if(tempDist > newDist)
			{
				temp 		= rect.getBottomLeft();
				tempDist	= newDist;
			}
			newDist = (rect.getBottomRight() - circle.position).magnatudeSquared();
			if(tempDist > newDist)
			{
				temp 		= rect.getBottomRight();
				tempDist	= newDist;
			}
			
			
			return Collision::test(temp, circle);
		}
		
	}
	template <class T>
	Result test( const Circle<T>& circle, const Rect<T>& rect){return -test(rect, circle);}
}

static void _getPositionedCollider(const Rect<double>& collider, Rect<double>& positionedCollider, const Vector2<double>& position, const Vector2<double>& scale, const double& rotation)
{
	positionedCollider = Rect<double>(position + collider.position, collider.size * scale);
}

static void _getPositionedCollider(const Circle<double>& collider, Circle<double>& positionedCollider, const Vector2<double>& position, const Vector2<double>& scale, const double& rotation)
{
	positionedCollider = Circle<double>(position + collider.position, scale.x * collider.radius);
}

static void _getPositionedCollider(const SimpleSegment<double>& collider, SimpleSegment<double>& positionedCollider, const Vector2<double>& position, const Vector2<double>& scale, const double& rotation)
{
	positionedCollider = SimpleSegment<double>(position + collider.position, scale.x * collider.length, collider.isVertical);
}

class Collider
{
public:
    virtual ~Collider(){};
    virtual Collision::Result test(const Collider&) const =0;
    virtual Collision::Result test(const Rect<double>&) const =0;
    virtual Collision::Result test(const Circle<double>&) const =0;
    virtual Collision::Result test(const SimpleSegment<double>&) const =0;
    virtual void updateCollider(const Transform&) =0;
    virtual void updateCollider(const Vector2<double>&, const Vector2<double>&, const double&) =0;
};

template <class T>
class ShapeCollider : public Collider
{
	
public:
	T positionedCollider;
public:
    T collider;
    ShapeCollider(T c)
        : collider(c), positionedCollider(c)
    {}

    virtual Collision::Result test(const Collider& c) const
    {
        return c.test(positionedCollider);
    }
    virtual Collision::Result test(const Rect<double>& c) const
    {
        return Collision::test(c, positionedCollider);
    }
    virtual Collision::Result test(const Circle<double>& c) const
    {
        return Collision::test(c, positionedCollider);
    }
    virtual Collision::Result test(const SimpleSegment<double>& c) const
    {
        return Collision::test(c, positionedCollider);
    }

    virtual void updateCollider(const Transform& transform)
    {
        _getPositionedCollider(collider, positionedCollider, transform.position, transform.scale, transform.rotation);
    }
    virtual void updateCollider(const Vector2<double>& position, const Vector2<double>& scale, const double& rotation)
    {
        _getPositionedCollider(collider, positionedCollider, position, scale, rotation);
    }

    virtual ~ShapeCollider(){};
};

typedef ShapeCollider<Rect<double> > RectCollider;
typedef ShapeCollider<Circle<double> > CircleCollider;
typedef ShapeCollider<SimpleSegment<double> > SimpleSegmentCollider;


#endif // COLISIONS_HPP_INCLUDED