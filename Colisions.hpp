#ifndef COLISIONS_HPP_INCLUDED
#define COLISIONS_HPP_INCLUDED

#include "Shapes.hpp"

using eType = double;

/*
            Pnt     SSeg    Rect    Circ
    Pnt              C      FC      FC
    SSeg     C       C       C       C
    Rect    FC       C      FC      FC
    Circ    FC       C      FC      FC



*/


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
		double distance     = 0;
		bool   value        = false;
		
		operator bool() const
		{
			return value;
		}
		
		Result operator -()
		{
			return Result(value, -xPenetration, -yPenetration, distance);
		}
		
		Vector2<double> getPenetrationVector() const
		{
			return Vector2<double>(xPenetration, yPenetration);
		}
		
		Result(bool v = false, double x = 0, double y = 0, double dist = 0)
			: xPenetration(x), yPenetration(y), distance(dist), value(v)
		{}
		Result(double x, double y, double dist = 0)
			: xPenetration(x), yPenetration(y), distance(dist), value(x != 0 || y != 0)
		{}
		
		static const Result& getBetter(const Result& r1, const Result& r2)
		{
			if(!r1)
				return r2;
			if(!r2);
				return r1;
			if(r1.getPenetrationVector().magnatudeSquared() <  r2.getPenetrationVector().magnatudeSquared())
				return r1;
			return r2;
		}
		static const Result& getWorse(const Result& r1, const Result& r2)
		{
			if(!r1)
				return r2;
			if(!r2);
				return r1;
			if(r1.getPenetrationVector().magnatudeSquared() >  r2.getPenetrationVector().magnatudeSquared())
				return r1;
			return r2;
		}
	};
	
	// Point - Range
	template <class T>
	Result test( const T value, const Vector2<T>& range)
	{
		T lower  = value - std::min(range.x, range.y);
		T higher = std::max(range.x, range.y) - value;
		
		return Result(lower >=0 && higher >= 0, 0, 0, lower<higher?-lower:higher);
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
		
		return Result( toLower >=0 && toHigher >= 0, 0, 0, (toLower<toHigher)?(-toLower):(toHigher));
	}
	
	// Point - SimpleSegment
	template <class T>
	Result test(const Vector2<T>& point, const SimpleSegment<T>& ssegment)
	{
	    Result r;
		if(ssegment.isVertical)
		{
			r = test(point.y, ssegment.getRange());
			r.distance = std::abs(point.x - ssegment.position.x);
		}
		else
        {            
			r = test(point.x, ssegment.getRange());
			r.distance = std::abs(point.y - ssegment.position.y);
        }
		return r;
	}
	template <class T>
	Result test( const SimpleSegment<T>& ssegment, const Vector2<T>& point){return -test(point, ssegment);}
	
	
	// Point - Rect
	template <class T>
	Result test(const Vector2<T>& point, const Rect<T>& rect)
	{
		Result horizontal 	= test(point.x, rect.getHorizontal());
		Result vertical 	= test(point.y, rect.getVertical());
		
		return Result(horizontal && vertical, horizontal.distance, vertical.distance);
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
			return Result(hResult && vResult, hResult.distance, 0, vResult.distance);
		}
        hResult = test(ssegment.getRange(), rect.getHorizontal());
        vResult = test(ssegment.position.y, rect.getVertical());
        return Result(hResult && vResult, 0, vResult.distance, hResult.distance);
	}
	template <class T>
	Result test( const Rect<T>& rect, const SimpleSegment<T>& ssegment){return -test(ssegment, rect);}
	
	// SimpleSegment - SimpleSegment
	template <class T>
	Result test(const SimpleSegment<T>& ssegment1, const SimpleSegment<T>& ssegment2, double maxDistance = -1)
	{
		if(ssegment1.isVertical != ssegment2.isVertical)
        {
            if(ssegment1.isVertical)
            {
                Result r1 = test(ssegment1.getRange(), ssegment2.position.y);
                Result r2 = test(ssegment1.position.x, ssegment2.getRange());
                
                return Result(r1 && r2, r2.xPenetration, r1.xPenetration);
            }
            else
            {
                Result r1 = test(ssegment1.getRange(), ssegment2.position.x);
                Result r2 = test(ssegment1.position.y, ssegment2.getRange());
                
                return Result(r1 && r2, r1.distance, r2.distance);
            }
        }
		else
        {
            Result rangeRes = test(ssegment1.getRange(), ssegment2.getRange());
            if(ssegment1.isVertical)
            {
                rangeRes.yPenetration   = rangeRes.distance;
                rangeRes.xPenetration   = 0;
                rangeRes.distance       = ssegment2.position.x - ssegment1.position.x;
            }
            else
            {
                rangeRes.yPenetration   = 0;
                rangeRes.xPenetration   = rangeRes.distance;
                rangeRes.distance       = ssegment2.position.y - ssegment1.position.y;
            }
            if(maxDistance >= 0){
                rangeRes.value = std::abs(rangeRes.distance) <= maxDistance; 
            }
            return rangeRes;
        }
	}
	
	// Rect - Rect
	template <class T>
	Result test(const Rect<T>& rect1, const Rect<T>& rect2)
	{
		Result horizontal 	= test(rect1.getHorizontal(), 	rect2.getHorizontal());
		Result vertical 	= test(rect2.getVertical(), 	rect2.getVertical());
		
		return Result(horizontal && vertical, horizontal.distance, vertical.distance);
	}
	
	// Point - Circle
	template <class T>
	Result test(const Vector2<T>& point, const Circle<T>& circle)
	{
		Vector2<T> penetration = point-circle.position;
		auto dist = std::abs(penetration.magnatude() - circle.radius);
		if(penetration.magnatudeSquared() <= circle.radius * circle.radius)
		{
			penetration.resizeSelf(circle.radius - penetration.magnatude());
			return Result(true, penetration.x, penetration.y, dist);
		}
		
		return Result(false, 0, 0, dist);
	}
	template <class T>
	Result test( const Circle<T>& circle, const Vector2<T>& point){return -test(point, circle);}
	
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
				if(std::abs(vResult.distance) < std::abs(hResult.distance))
				{
					return Result(true, 0, vResult.distance);
				}
				else
				{
					return Result(true, hResult.distance, 0);
				}
			}
			else
			{
				return Result(true, 0, vResult.distance);
			}
		}
		else if(hResult)
		{
			return Result(true, hResult.distance, 0);
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
	
	// SimpleSegment - Circle
	template <class T>
	Result test(const SimpleSegment<T>& ssegment, const Circle<T>& circle)
	{
	    Result simpleRes = Collision::test(ssegment.isVertical ? circle.position.y : circle.position.x, ssegment.getRange());
	    auto distance = Collision::test(ssegment.isVertical ? circle.getRangeY() : circle.getRangeX(), ssegment.getRange()).distance;
        if(ssegment.isVertical && simpleRes)
        {
            auto dist = std::abs(circle.position.x - ssegment.position.x);
            return Result(dist <= circle.radius, (circle.position.x < ssegment.position.x ? 1 : -1) * (circle.radius - dist), 0, distance);
        }
        if(!ssegment.isVertical && simpleRes)
        {
            auto dist = std::abs(circle.position.y - ssegment.position.y);
            return Result(dist <= circle.radius, 0, (circle.position.y < ssegment.position.y ? 1 : -1) * (circle.radius - dist), distance);
        }
        Result r = Result::getWorse(test(ssegment.position, circle), test(ssegment.getEnd(), circle));
        r.distance = distance;
        return r;		
	}
	template <class T>
	Result test( const Circle<T>& circle, const SimpleSegment<T>& ssegment){return -test(ssegment, circle);}
}

class Collider;

class Collidable
{
public:
    virtual const Collider* getCollider() const = 0;
};

class Collider : public Collidable
{
protected:
    
    
    static void _getPositionedCollider(const Rect<double>& collider, Rect<double>& positionedCollider, const Vector2<double>& position, const Vector2<double>& scale, double rotation)
    {
        int rotations = 0;
        while(rotation > 45)
        {
            rotations++;
            rotation -= 90;
        }
        while(rotation < -45)
        {
            rotations--;
            rotation += 90;
        }
        
        Rect<double> newCollider = collider.rotate(rotations);
        
        newCollider.position *= scale;
        newCollider.size *= scale;
        
        if(newCollider.size.x < 0)
        {
            newCollider.position.x -= newCollider.size.x;
            newCollider.size.x *= -1;
        }
        if(newCollider.size.y < 0)
        {
            newCollider.position.y -= newCollider.size.y;
            newCollider.size.y *= -1;
        }
        
        newCollider.position += position;
        if(tak)
            std::cout << newCollider.position << " " << newCollider.size << " Rotations: " << rotations << std::endl;
        
        positionedCollider = newCollider;
        
    }

    static void _getPositionedCollider(const Circle<double>& collider, Circle<double>& positionedCollider, const Vector2<double>& position, const Vector2<double>& scale, double rotation)
    {
        positionedCollider = Circle<double>((position * scale).rotate(rotation) + collider.position, std::abs(scale.x) * collider.radius);
    }

    static void _getPositionedCollider(const SimpleSegment<double>& collider, SimpleSegment<double>& positionedCollider, const Vector2<double>& position, const Vector2<double>& scale, double rotation)
    {
        // TODO REPAIR
        positionedCollider = SimpleSegment<double>((position * scale) + collider.position, scale.x * collider.length, collider.isVertical);
    }
    
public:
    virtual const Collider* getCollider() const
    {
        return this;
    }
    
    virtual ~Collider(){};
    virtual Collider* clonePtr() const = 0;
    virtual Collision::Result test(const Collider&) const =0;
    virtual Collision::Result test(const Rect<double>&) const =0;
    virtual Collision::Result test(const Circle<double>&) const =0;
    virtual Collision::Result test(const SimpleSegment<double>&) const =0;
    virtual void updateCollider(const Vector2<double>& position = Vectors::null, const Vector2<double>& scale = Vectors::units, double rotation = 0) =0;
    void updateCollider(const Transformable& transform)
    {
        updateCollider(transform.getPosition(), transform.getScale(), transform.getRotation());
    }
    virtual Collision::Result test(const Collider* c) const 
    {
    	if(c)
		{
			return test(*c);
		}
		return Collision::Result(false);
    }
    
};

template <class T>
class ShapeCollider : public Collider
{
	
	T positionedCollider;
public:
    T collider;
    ShapeCollider(T c)
        : positionedCollider(c), collider(c)
    {}
    
    virtual Collider* clonePtr() const
    {
    	Collider* res = new ShapeCollider<T>(collider);
    	return res;
    }

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
    virtual void updateCollider(const Vector2<double>& position = Vectors::null, const Vector2<double>& scale = Vectors::units, double rotation = 0)
    {
        _getPositionedCollider(collider, positionedCollider, position, scale, rotation);
    }
    virtual ~ShapeCollider(){};
    
    T getPositionedCollider()
    {
        return positionedCollider;
    }
};

template <class T>
class FixedShapeCollider : public Collider
{
	
public:
    T collider;
    FixedShapeCollider(T c)
        : collider(c)
    {}
    
    virtual Collider* clonePtr() const
    {
    	Collider* res = new FixedShapeCollider<T>(collider);
    	return res;
    }

    virtual Collision::Result test(const Collider& c) const
    {
        return c.test(collider);
    }
    virtual Collision::Result test(const Rect<double>& c) const
    {
        return Collision::test(c, collider);
    }
    virtual Collision::Result test(const Circle<double>& c) const
    {
        return Collision::test(c, collider);
    }
    virtual Collision::Result test(const SimpleSegment<double>& c) const
    {
        return Collision::test(c, collider);
    }
    virtual void updateCollider(const Vector2<double>& position = Vectors::null, const Vector2<double>& scale = Vectors::units, double rotation = 0)
    {
        return;
    }
    virtual ~FixedShapeCollider(){};
};

typedef ShapeCollider<Rect<double> >                RectCollider;
typedef ShapeCollider<Circle<double> >              CircleCollider;
typedef ShapeCollider<SimpleSegment<double> >       SimpleSegmentCollider;

typedef FixedShapeCollider<Rect<double> >           FixedRectCollider;
typedef FixedShapeCollider<Circle<double> >         FixedCircleCollider;
typedef FixedShapeCollider<SimpleSegment<double> >  FixedSimpleSegmentCollider;

template<typename TObject1, typename TObject2, typename THandler>
void handleCollision(TObject1& object1, TObject2& object2, THandler handler)
{
    Collision::Result result = object1.getCollider()->test(object2.getCollider());
    if(result)
    {
		handler(result, object1, object2);
    }
}

template<typename TObject, typename TFwdIterator, typename THandler>
void handleAllCollisions(TObject& object, TFwdIterator begin, TFwdIterator end, THandler handler)
{
    for(auto it = begin; it < end; it++)
    {
        handleCollision(object, *it, handler);
    }
}
    





template<typename T1, typename T2>
Vector2d moveOutOfCollision(const Collision::Result& result, T1& object1, T2& object2)
{
	Vector2d shift = result.getPenetrationVector();
    object1.move(shift);
    return shift;
}

template<typename T1>
Vector2d moveOutOfWall(const Collision::Result& result, T1& object)
{
	Vector2d shift = result.getPenetrationVector();
    object.move(shift);
	return shift;
}
template<typename T1>
void bounceOutOfWall(const Collision::Result& result, T1& object, SimpleSegment<double>& wall, double bounce = 1)
{
    if(wall.isVertical){
        object.move(Vector2d(result.xPenetration, 0));
        return;
    }
    object.move(Vector2d(0, result.yPenetration));
}
#endif // COLISIONS_HPP_INCLUDED
