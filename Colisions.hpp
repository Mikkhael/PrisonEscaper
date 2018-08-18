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
	
    std::ostream& operator<<(std::ostream& cout, const Result& r)
    {
        cout << "Result: " << r.value << "   " << r.getPenetrationVector() << " - " << r.distance;
        return cout; 
    }

	
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
                
                Result rx = test(Vector2<T>(line1.point1.x, line1.point2.x), Vector2<T>(line2.point1.x, line2.point2.x));
                Result ry = test(Vector2<T>(line1.point1.y, line1.point2.y), Vector2<T>(line2.point1.y, line2.point2.y));
                
                return Result(rx && ry, rx.distance, ry.distance, std::sqrt(rx*rx+ ry*ry));
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
            if(i == poly.points.size())
            {
                normal = line.toVector().rotate90().normalize();
            }
            else
            {
                normal = poly.getEdgeVector(i).rotate90().normalize();
            }
            
            Vector2d polyProjection;
            for(unsigned int j=0; j<poly.points.size(); j++)
            {
                double dot = poly.getPoint(j).dot(normal);
                if(j == 0)
                {
                    polyProjection = Vector2d(dot, dot);
                }
                else
                {
                    if(dot < polyProjection.x)
                    {
                        polyProjection.x = dot;
                    }
                    else if(dot > polyProjection.y)
                    {
                        polyProjection.y = dot;
                    }
                }
            }
            
            Vector2d lineProjection = Vector2d(line.point1.dot(normal), line.point2.dot(normal));
            
            Result projectionResult = test(polyProjection, lineProjection);
            if(projectionResult)
            {
                Vector2d tempMtv = projectionResult.distance * normal;
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
                normal = poly2.getEdgeVector(i - poly1.points.size()).rotate90().normalize();
            }
            else
            {
                normal = poly1.getEdgeVector(i).rotate90().normalize();
            }
            
            Vector2d polyProjection1;
            for(unsigned int j=0; j<poly1.points.size(); j++)
            {
                double dot = poly1.getPoint(j).dot(normal);
                if(j == 0)
                {
                    polyProjection1 = Vector2d(dot, dot);
                }
                else
                {
                    if(dot < polyProjection1.x)
                    {
                        polyProjection1.x = dot;
                    }
                    else if(dot > polyProjection1.y)
                    {
                        polyProjection1.y = dot;
                    }
                }
            }
            
            Vector2d polyProjection2;
            for(unsigned int j=0; j<poly2.points.size(); j++)
            {
                double dot = poly2.getPoint(j).dot(normal);
                if(j == 0)
                {
                    polyProjection2 = Vector2d(dot, dot);
                }
                else
                {
                    if(dot < polyProjection2.x)
                    {
                        polyProjection2.x = dot;
                    }
                    else if(dot > polyProjection2.y)
                    {
                        polyProjection2.y = dot;
                    }
                }
            }
            
            
            Result projectionResult = test(polyProjection1, polyProjection2);
            if(projectionResult)
            {
                Vector2d tempMtv = projectionResult.distance * normal;
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
            Vector2d normal;
            normal = poly.getEdgeVector(i).rotate90().normalize();
            
            Vector2d polyProjection;
            for(unsigned int j=0; j<poly.points.size(); j++)
            {
                double dot = poly.getPoint(j).dot(normal);
                if(j == 0)
                {
                    polyProjection = Vector2d(dot, dot);
                }
                else
                {
                    if(dot < polyProjection.x)
                    {
                        polyProjection.x = dot;
                    }
                    else if(dot > polyProjection.y)
                    {
                        polyProjection.y = dot;
                    }
                }
            }
            
            double circleCenterProjection = circle.position.dot(normal);
            Vector2d circleProjection = Vector2d(circleCenterProjection - circle.radius, circleCenterProjection + circle.radius);
            
            Result projectionResult = test(polyProjection, circleProjection);
            if(projectionResult)
            {
                Vector2d tempMtv = projectionResult.distance * normal;
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
