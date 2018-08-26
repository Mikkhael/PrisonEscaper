#ifndef COLISIONS_HPP_INCLUDED
#define COLISIONS_HPP_INCLUDED

#include "Shapes.hpp"

using eType = double;

/*
            Pnt     SSeg    Rect    Circ    Line    Poly
    Pnt              C      FC      FC              FC
    SSeg     C       C       C       C       C       C
    Rect    FC       C      FC      FC       C       C
    Circ    FC       C      FC      FC       C       C
    Line             C       C       C       C       C
    Poly    FC       C       C       C       C       C



*/


namespace CollisionFast
{
	
	// Value in Range
    template<class A, class B>
    bool valueInRange(const Vector2<A>& seg, B point, eType e = 0.0)
    {
    	A mx = std::max(seg.x, seg.y);
    	A mn = std::min(seg.x, seg.y);
    	
    	if(e*2 >= std::abs(mx - mn))
			return false;
        
		return  std::min(seg.x+e, seg.y-e) <= point &&
				std::max(seg.x+e, seg.y-e) >= point;
    }
    template<class A, class B>
    bool valueInRange(B point, const Vector2<A>& seg, eType e = 0.0){return valueInRange(seg,point,e);}

	// Range intersection
    template<class A, class B>
    bool rangeIntersection(const Vector2<A>& a, const Vector2<B>& b, eType e = 0.0)
    {
        return  std::min(a.x, a.y) + e < std::max(b.x, b.y) &&
				std::max(a.x, a.y) - e > std::min(b.x, b.y);
    }
	
	// Point in Rect
    template<class A, class B>
    bool test(const Rect<A>& a, const Vector2<B>& b, eType eHorizontal = 0.0, eType eVertical = 0.0)
    {
        return  valueInRange(a.getHorizontal(), b.x, eHorizontal) &&
                valueInRange(a.getVertical(),   b.y, eVertical);
    }
    template<class A, class B>
    bool test(const Vector2<B>& b, const Rect<A>& a, eType eHorizontal = 0.0, eType eVertical = 0.0){return test(a, b, eHorizontal, eVertical);}

	// Rect intersection
    template<class A, class B>
    bool test(const Rect<A>& a, const Rect<B>& b, eType eHorizontal = 0.0, eType eVertical = 0.0)
    {
        return  rangeIntersection(a.getHorizontal(), b.getHorizontal(), eHorizontal) &&
                rangeIntersection(a.getVertical(),   b.getVertical(),   eVertical);
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
        return  (valueInRange(b.getHorizontal(), a.center.x, eHorizontal) && valueInRange(Vector2<B>(b.position.y-a.radius,  b.position.y+b.size.y+a.radius),  a.center.y, eVertical  )) ||
                (valueInRange(b.getVertical(),   a.center.y, eVertical  ) && valueInRange(Vector2<B>(b.position.x-a.radius,  b.position.x+b.size.x+a.radius),  a.center.x, eHorizontal)) ||
                test(a, b.getUpperLeft(),   eCorner) ||
                test(a, b.getUpperRight(),  eCorner) ||
                test(a, b.getBottomLeft(),  eCorner) ||
                test(a, b.getBottomRight(), eCorner);
    }
    template<class A, class B>
    bool test(const Rect<B>& b, const Circle<A>& a, eType eHorizontal = 0.0, eType eVertical = 0.0){return test(a, b, eHorizontal, eVertical);}
    
    
    // Point - Poly
    template<class A, class B>
    bool test(const Vector2<A>& point, const Polygon<B>& poly)
    {
        if(!poly.isValid())
        {
            return false;
        }
        
        Line<B> edge;
        bool res = false;
        for (unsigned int i = 0; i < poly.points.size(); i++) 
        {
            edge = poly.getEdgeLine(i);
            if ( ((edge.point2.y > point.y) != (edge.point1.y > point.y)) &&
                 (point.x < (edge.point1.x - edge.point2.x) * (point.y - edge.point2.y) / (edge.point1.y - edge.point2.y) + edge.point2.x) )
            {
                res = !res;
            }
        }
        return res;
    }
    template<class A, class B>
    bool test(const Polygon<B>& b, const Vector2<A>& a){return test(a, b);}
    
};

namespace Collision
{
	struct Result
	{
		double xPenetration = 0;
		double yPenetration = 0;
		double distance     = 0;
		bool   value        = false;
		
		void updateValue()
		{
		    if(xPenetration == 0 && yPenetration == 0)
            {
                value = false;
            }
		}
		
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
		
		Result(bool value_ = false)
            : value(value_)
		{}
		
		Result(bool v, double x, double y, double dist = 0)
			: xPenetration(x), yPenetration(y), distance(dist), value(v)
		{
            updateValue();
		}
		
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
	
    std::ostream& operator<<(std::ostream& cout, const Result& r)
    {
        cout << "Result: " << r.value << "   " << r.getPenetrationVector() << " - " << r.distance;
        return cout; 
    }
	
	struct Overlap
	{
	    bool isOverlaping = false;
	    double value = 0;
	    
	    operator bool() const
	    {
	        return isOverlaping;
	    }
	    
	    Overlap(bool isOverlaping_ = 0, double value_ = 0)
            : isOverlaping(isOverlaping_) , value(value_)
        {}
	};
	
	// Point - Range
	template <class T>
	Overlap valueInRange( const T value, const Vector2<T>& range)
	{
		T lower  = value - std::min(range.x, range.y);
		T higher = std::max(range.x, range.y) - value;
		
		return Overlap(lower >=0 && higher >= 0, lower<higher ? -lower : higher);
	}
	template <class T>
	Overlap valueInRange(const Vector2<T>& range, const T value){return -valueInRange(value, range);}
	
	// Range - Range
	template <class T>
	Overlap rangeIntersection(const Vector2<T>& range1, const Vector2<T>& range2)
	{
		Vector2<T> r1 = range1.sort();
		Vector2<T> r2 = range2.sort();
		
		T toLower  = r1.y - r2.x;
		T toHigher = r2.y - r1.x;
		
		return Overlap( toLower >=0 && toHigher >= 0 , toLower<toHigher ? -toLower : toHigher);
	}
		
	// Point - Rect
	template <class T>
	Result test(const Vector2<T>& point, const Rect<T>& rect)
	{
		Overlap horizontal 	= valueInRange(point.x, rect.getHorizontal());
		Overlap vertical 	= valueInRange(point.y, rect.getVertical());
		
		return Result(horizontal && vertical, horizontal.value, vertical.value);
	}
	template <class T>
	Result test( const Rect<T>& rect, const Vector2<T>& point){return -test(point, rect);}
	
	
	// SimpleSegment - Rect
	template <class T>
	Result test(const SimpleSegment<T>& ssegment, const Rect<T>& rect)
	{
		Overlap hResult;
		Overlap vResult;
		if(ssegment.isVertical)
		{
			hResult = valueInRange(ssegment.position.x, rect.getHorizontal());
			vResult = rangeIntersection(ssegment.getRange(), rect.getVertical());
			
			return Result(hResult && vResult, hResult.value, 0, vResult.value);
		}
        hResult = rangeIntersection(ssegment.getRange(), rect.getHorizontal());
        vResult = valueInRange(ssegment.position.y, rect.getVertical());
        
        return Result(hResult && vResult, 0, vResult.value, hResult.value);
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
                Overlap r1 = valueInRange(ssegment1.getRange(), ssegment2.position.y);
                Overlap r2 = valueInRange(ssegment1.position.x, ssegment2.getRange());
                
                return Result(r1 && r2, r2.value, r1.value);
            }
            else
            {
                Overlap r1 = valueInRange(ssegment1.getRange(), ssegment2.position.x);
                Overlap r2 = valueInRange(ssegment1.position.y, ssegment2.getRange());
                
                return Result(r1 && r2, r1.value, r2.value);
            }
        }
		else
        {
            Result result;
            Overlap rangeRes = rangeIntersection(ssegment1.getRange(), ssegment2.getRange());
            result.value = rangeRes.isOverlaping;
            if(ssegment1.isVertical)
            {
                result.yPenetration   = rangeRes.value;
                result.xPenetration   = 0;
                result.distance       = ssegment2.position.x - ssegment1.position.x;
            }
            else
            {
                result.yPenetration   = 0;
                result.xPenetration   = rangeRes.value;
                result.distance       = ssegment2.position.y - ssegment1.position.y;
            }
            if(maxDistance >= 0){
                result.value = std::abs(result.distance) <= maxDistance; 
            }
            result.updateValue();
            return result;
        }
	}
	
	// Rect - Rect
	template <class T>
	Result test(const Rect<T>& rect1, const Rect<T>& rect2)
	{
		Overlap horizontal 	= rangeIntersection(rect1.getHorizontal(), 	rect2.getHorizontal());
		Overlap vertical 	= rangeIntersection(rect2.getVertical(), 	rect2.getVertical());
		
		return Result(horizontal && vertical, horizontal.value, vertical.value);
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
		Overlap vResult(false);
		Overlap hResult(false);
		if(valueInRange(rect.getHorizontal(), circle.position.x))
		{
			vResult = valueInRange(Vector2<T>(rect.position.y - circle.radius, rect.position.y + rect.size.y + circle.radius), circle.position.y);
		}
		if(valueInRange(rect.getVertical(),   circle.position.y))
		{
			hResult = valueInRange(Vector2<T>(rect.position.x - circle.radius, rect.position.x + rect.size.x  + circle.radius), circle.position.x);
		}
		
		if(vResult)
		{
			if(hResult)
			{
				if(std::abs(vResult.value) < std::abs(hResult.value))
				{
					return Result(true, 0, vResult.value);
				}
				else
				{
					return Result(true, hResult.value, 0);
				}
			}
			else
			{
				return Result(true, 0, vResult.value);
			}
		}
		else if(hResult)
		{
			return Result(true, hResult.value, 0);
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
			
			
			return test(temp, circle);
		}
		
	}
	template <class T>
	Result test( const Circle<T>& circle, const Rect<T>& rect){return -test(rect, circle);}
	
	// SimpleSegment - Circle
	template <class T>
	Result test(const SimpleSegment<T>& ssegment, const Circle<T>& circle)
	{
	    Overlap simpleRes = valueInRange(ssegment.isVertical ? circle.position.y : circle.position.x, ssegment.getRange());
	    double  distance  = rangeIntersection(ssegment.isVertical ? circle.getRangeY() : circle.getRangeX(), ssegment.getRange()).value;
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
	
	// Line - Line
	template <class T>
	Result test(const Line<T>& line1, const Line<T>& line2)
	{
        Vector2<T> pDiff = line2.point1 - line1.point1;
        double vCross = line1.toVector().cross(line2.toVector());
        if(vCross == 0)
        {
            if(pDiff.cross(line1.toVector()) == 0)
            {
                
                Overlap rx = rangeIntersection(Vector2<T>(line1.point1.x, line1.point2.x), Vector2<T>(line2.point1.x, line2.point2.x));
                Overlap ry = rangeIntersection(Vector2<T>(line1.point1.y, line1.point2.y), Vector2<T>(line2.point1.y, line2.point2.y));
                
                return Result(rx && ry, rx.value, ry.value, std::sqrt(rx*rx+ ry*ry));
            }
            else
            {
                return Result(false);
            }
        }
        else
        {
            double s1 = pDiff.cross(line2.toVector()) / vCross;
            double s2 = pDiff.cross(line1.toVector()) / vCross;
            
            Vector2d penetration;
            if(s1 > 0.5)
            {
                penetration = (line1.point1 + line1.toVector() * s1) - line1.point2;
            }
            else
            {
                penetration = line1.toVector() * -s1;
            }
            double distance = ((s2 > 0.5 ? 1-s2 : s2) * line2.toVector()).magnatude();
            
            return Result(s1 >= 0 && s1 <= 1 && s2 >= 0 && s2 <= 1, penetration.x, penetration.y, distance);
        }
	}
	
	// Poly - Point
	template <class T>
	Result test(const Polygon<T>& poly, const Vector2<T>& point)
	{
        if(!CollisionFast::test(poly, point))
        {
            return Result(false);
        }
        
        if(poly.isConvex)
        {
            Vector2d mtv = Vectors::null;
            double bestCross = 0;
            double minDistance = -1;
            bool set = false;
            
            for(unsigned int i=0; i<poly.points.size(); i++)
            {
                Line<T> edge = poly.getEdgeLine(i);
                Vector2d pointVector = point - edge.point1;
                Vector2d edgeVector = edge.toVector();
                Vector2d edgeVectorUnit = edgeVector.normalize();
                
                long double mag2 = pointVector.magnatudeSquared();
                if(minDistance < 0 || minDistance > mag2)
                {
                    minDistance = mag2;
                }
                
                double tempCross = pointVector.cross(edgeVectorUnit);
                if(!set || std::abs(bestCross) > std::abs(tempCross))
                {
                    set = true;
                    double temp = edgeVectorUnit.x;
                    edgeVectorUnit.x = edgeVectorUnit.y;
                    edgeVectorUnit.x = -temp;
                    
                    mtv = edgeVectorUnit * tempCross;
                    bestCross = tempCross;
                }
            }
            return Result(true, mtv.x, mtv.y, bestCross, std::sqrt(minDistance));
        }
        
        return Result(false);
	}
	template <class T>
	Result test(const Vector2<T>& point, const Polygon<T>& poly){return -test(poly, point);}
	
	// Poly - Line
	template <class T>
	Result test(const Polygon<T>& poly, const Line<T>& line)
	{
	    if(!poly.isValid() || !poly.isConvex)
        {
            return Result(false);
        }
        
        Vector2d mtv;
        bool mtvSet = false;
        
	    for(unsigned int i=0; i < poly.points.size()+1; i++)
        {
            Vector2d normal;
            if(i == 0)
            {
                normal = line.toVector().rotate90().normalize();
            }
            else
            {
                normal = poly.getNormal(i);
            }
            
            Vector2d polyProjection = poly.getProjectionRange(normal);            
            Vector2d lineProjection = Vector2d(line.point1.dot(normal), line.point2.dot(normal));
            
            Overlap projectionResult = rangeIntersection(polyProjection, lineProjection);
            if(projectionResult)
            {
                Vector2d tempMtv = projectionResult.value * normal;
                if(!mtvSet || mtv.magnatudeSquared() > tempMtv.magnatudeSquared())
                {
                    mtvSet = true;
                    mtv = tempMtv;
                }
            }
            else
            {
                return Result(false);
            }
        }
        return Result(true, mtv.x, mtv.y, 100);
	}
	template <class T>
	Result test(const Line<T>& line, const Polygon<T>& poly){return -test(poly, line);}
	
	// Poly - Poly
	template <class T>
	Result test(const Polygon<T>& poly1, const Polygon<T>& poly2)
	{
        if(!poly1.isValid() || !poly1.isConvex || !poly2.isValid() || !poly2.isConvex)
        {
            return Result(false);
        }
        
        Vector2d mtv;
        bool mtvSet = false;
        
	    for(unsigned int i=0; i < poly1.points.size()+poly2.points.size(); i++)
        {
            Vector2d normal;
            if(i >= poly1.points.size())
            {
                normal = poly2.getNormal(i - poly1.points.size());
            }
            else
            {
                normal = poly1.getNormal(i);
            }
            
            
            Vector2d polyProjection1 = poly1.getProjectionRange(normal);
            Vector2d polyProjection2 = poly2.getProjectionRange(normal);
            
            
            Overlap projectionResult = rangeIntersection(polyProjection1, polyProjection2);
            if(projectionResult)
            {
                Vector2d tempMtv = projectionResult.value * normal;
                if(!mtvSet || mtv.magnatudeSquared() > tempMtv.magnatudeSquared())
                {
                    mtvSet = true;
                    mtv = tempMtv;
                }
            }
            else
            {
                return Result(false);
            }
        }
        
        return Result(true, mtv.x, mtv.y, 100);
	}
	
	// Line - Rect
	template <class T>
	Result test(const Line<T>& line, const Rect<T>& rect)
	{
        return test(line, rect.toPolygon());
	}
	template <class T>
	Result test(const Rect<T>& rect, const Line<T>& line){return -test(line, rect);}
	
	// Line - Circle
	template <class T>
	Result test(const Line<T>& line, const Circle<T>& circle)
	{
        Vector2d a = circle.position - line.point1;
        Vector2d b = line.toVector();
        Vector2d b_unit = b.normalize();
        
        double asin = a.cross(b_unit);
        if(std::abs(asin) > circle.radius)
        {
            return Result(false);
        }
        
        double acos = a.dot(b_unit);
        if(acos <= 0)
        {
            return test(line.point1, circle);
        }
        if(acos > line.length())
        {
            return test(line.point2, circle);
        }
        
        return test(line.point1 + b_unit * acos, circle);
        
	}
	template <class T>
	Result test(const Circle<T>& circle, const Line<T>& line){return -test(line, circle);}
	
	// Line - SSegment
	template <class T>
	Result test(const Line<T>& line, const SimpleSegment<T>& ssegment)
	{
        return test(line, ssegment.toLine());
	}
	template <class T>
	Result test(const SimpleSegment<T>& ssegment, const Line<T>& line){return -test(line, ssegment);}
	
	// Poly - Rect
	template <class T>
	Result test(const Polygon<T>& poly, const Rect<T>& rect)
	{
        return test(poly, rect.toPolygon());
	}
	template <class T>
	Result test(const Rect<T>& rect, const Polygon<T>& poly){return -test(poly, rect);}
	
	// Poly - Circle
	template <class T>
	Result test(const Polygon<T>& poly, const Circle<T>& circle)
	{
        if(!poly.isValid() || !poly.isConvex)
        {
            return Result(false);
        }
        
        Vector2d mtv;
        bool mtvSet = false;
        
	    for(unsigned int i=0; i < poly.points.size(); i++)
        {
            Vector2d normal = poly.getNormal(i);
            
            Vector2d polyProjection = poly.getProjectionRange(normal);
            
            double circleCenterProjection = circle.position.dot(normal);
            Vector2d circleProjection = Vector2d(circleCenterProjection - circle.radius, circleCenterProjection + circle.radius);
            
            Overlap projectionResult = rangeIntersection(polyProjection, circleProjection);
            if(projectionResult)
            {
                Vector2d tempMtv = projectionResult.value * normal;
                if(!mtvSet || mtv.magnatudeSquared() > tempMtv.magnatudeSquared())
                {
                    mtvSet = true;
                    mtv = tempMtv;
                }
            }
            else
            {
                return Result(false);
            }
        }
        
        return Result(true, mtv.x, mtv.y, 100);		
	}
	template <class T>
	Result test(const Circle<T>& circle, const Polygon<T>& poly){return -test(poly, circle);}
	
	// Poly - SSegment
	template <class T>
	Result test(const Polygon<T>& poly, const SimpleSegment<T>& ssegment)
	{
        return test(poly, ssegment.toLine() );
	}
	template <class T>
	Result test(const SimpleSegment<T>& ssegment, const Polygon<T>& poly){return -test(poly, ssegment);}
	
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
    
    static void _getPositionedCollider(const Line<double>& collider, Line<double>& positionedCollider, const Vector2<double>& position, const Vector2<double>& scale, double rotation)
    {
        
        Line<double> newCollider = collider;
        newCollider.scaleSelf(scale);
        newCollider.rotateSelf(rotation);
        newCollider.moveSelf(position);
        
        positionedCollider = newCollider;
    }
    
    static void _getPositionedCollider(const Polygon<double>& collider, Polygon<double>& positionedCollider, const Vector2<double>& position, const Vector2<double>& scale, double rotation)
    {
        
        Polygon<double> newCollider = collider;
        newCollider.scaleSelf(scale);
        newCollider.rotateSelf(rotation);
        newCollider.moveSelf(position);
        
        positionedCollider = newCollider;
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
    virtual Collision::Result test(const Line<double>&) const =0;
    virtual Collision::Result test(const Polygon<double>&) const =0;
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
    virtual Collision::Result test(const Line<double>& c) const
    {
        return Collision::test(c, positionedCollider);
    }
    virtual Collision::Result test(const Polygon<double>& c) const
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
    virtual Collision::Result test(const Line<double>& c) const
    {
        return Collision::test(c, collider);
    }
    virtual Collision::Result test(const Polygon<double>& c) const
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
typedef ShapeCollider<Line<double> >                LineCollider;
typedef ShapeCollider<Polygon<double> >             PolygonCollider;

typedef FixedShapeCollider<Rect<double> >           FixedRectCollider;
typedef FixedShapeCollider<Circle<double> >         FixedCircleCollider;
typedef FixedShapeCollider<SimpleSegment<double> >  FixedSimpleSegmentCollider;
typedef FixedShapeCollider<Line<double> >           FixedLineCollider;
typedef FixedShapeCollider<Polygon<double> >        FixedPolygonCollider;

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
