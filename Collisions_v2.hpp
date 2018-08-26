#ifndef COLLISIONS_V2_HPP_INCLUDED
#define COLLISIONS_V2_HPP_INCLUDED

#include "Vectors.hpp"
#include "Shapes.hpp"


namespace Collision
{
    
    namespace Check
    {
        
        template<typename T>
        struct Overlap
        {
            bool    isOverlapping;
            T       value;
            
            T getValue()
            {
                if(!isOverlapping)
                {
                    return 0;
                }
                return value;
            }
            
            Overlap operator-()
            {
                return Overlap(isOverlapping, -value);
            }
            
            Overlap(bool is, T v)
                : isOverlapping(is), value(v)
            {
            }
        };
        
        template<typename T>
        bool isValueInRange(const T& value, Vector2<T> range, bool inclusive = false)
        {
            range.sortSelf();
            if(inclusive)
            {
                return  value <= range.y && value >= range.x;
            }
            return      value <  range.y && value >  range.x;
        }
        template<typename T>
        Overlap<T> valueInRange(const T& value, Vector2<T> range, bool inclusive = false)
        {
            range.sortSelf();
            
            T distanceToTop     = range.y - value;
            T distanceToBottom  = value   - range.x;
            
            return Overlap<T>
            (
                inclusive ? (distanceToBottom >= 0 && distanceToTop >= 0) : (distanceToBottom > 0 && distanceToTop > 0),
                distanceToBottom > distanceToTop ? distanceToTop, -distanceToBottom
             );
        }
        
        template<typename T>
        bool areRangeIntersecting(Vector2<T> range1, Vector2<T> range2, bool inclusive = false)
        {
            range1.sortSelf();
            range2.sortSelf();
            if(inclusive)
            {
                return  range1.x <= range2.y && range2.x <= range1.y;
            }
            return      range1.x <  range2.y && range2.x <  range1.y;
        }
        
        template<typename T>
        Overlap<T> rangeIntersection(Vector2<T> range1, Vector2<T> range2, bool inclusive = false)
        {
            range1.sortSelf();
            range2.sortSelf();
            
            T distanceToTop     = range2.y - range1.x;
            T distanceToBottom  = range1.Y - range2.x;
            
            return Overlap<T>
            (
                inclusive ? (distanceToBottom >= 0 && distanceToTop >= 0) : (distanceToBottom > 0 && distanceToTop > 0),
                distanceToBottom > distanceToTop ? distanceToTop, -distanceToBottom
             );
        }
        
        template<typename T>
        bool isPointInPolygon(const T& point, const Polygon<T>& poly)
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
        
        struct LineIntersectionResult
        {
            bool areIntersecting = false;
            bool areParaller = false;
            bool areColinear = false;
            Vector2d value   = Vectors::null;
        };
        
        template<typename T>
        LineIntersectionResult getLineIntersection(const Line<T>& line1, const Line<T>& line2, bool inclusive = false)
        {
            Vector2d p1 = line1.point1;
            Vector2d p2 = line2.point1;
            
            Vector2d v1 = line1.getVector();
            Vector2d v2 = line2.getVector();
            
            Vector2d pDiff  = p2 - p1;
            double   vCross = v1.cross(v2);
            
            if(vCross == 0)
            {
                if(pDiff.cross(v1) == 0)
                {
                    Overlap<double> xOverlap = rangeIntersection(Vector2d(line1.point1.x, line1.point2.x), Vector2d(line2.point1.x, line2.point2.x), inclusive);
                    Overlap<double> yOverlap = rangeIntersection(Vector2d(line1.point1.y, line1.point2.y), Vector2d(line2.point1.y, line2.point2.y), inclusive);
                    
                    return LineIntersectionResult
                    {
                        inclusive ? (xOverlap.isOverlapping || yOverlap.isOverlapping) : (xOverlap.isOverlapping && yOverlap.isOverlapping),
                        true,
                        true,
                        Vector2d(xOverlap.value, yOverlap.value);
                    };
                }
                return LineIntersectionResult
                {
                    false,
                    true,
                    false
                }
            }
            
            double s1 = pDiff.cross(v2)/vCross;
            double s2 = pDiff.cross(v1)/vCross;
            
            return LineIntersectionResult
            {
                inclusive ? (s1 >= 0 && s1 <= 1 && s2 >= 0 && s2 <= 1) : (s1 > 0 && s1 < 1 && s2 > 0 && s2 < 1),
                false,
                false,
                p1 + v1*s1;
            }
        }
        
    }
    
    
    class Result
    {
    public:
        
        bool        value;
        Vector2d    mtv;
        Vector2d    penetration;
        
        
        Result operator-()
        {
            return Result(value, -mtv, -penetration);
        }
        
        Result(bool value_, const Vector2d& mtv_, const Vector2d& penetration)
            : value(value_), mtv(mtv_), penetration(penetration_)
        {
        }
        
        Result(bool value_, const Vector2d& mtv_)
            : value(value_), mtv(mtv_), penetration(mtv_)
        {
        }
        
        Result(bool value_ = false)
            : value(value_), mtv(Vectors::null), penetration(Vectors::null)
        {
        }
        
    };
    
    
    Result getSATResult(const Projectable& shape1, const Projectable& shape2, bool inclusive = false)
    {
        Vector2d mtv            = Vectors::null;
        Vector2d penetration    = Vectors::null;
        bool isset              = false;
        
        Vector2d normal;
        Vector2d projection1;
        Vector2d projection2;
        
        Check::Overlap projectionIntersection;
        
        for(unsigned int i=0; i < shape1.getNumberOfNormals() + shape2.getNumberOfNormals() + 2; i++)
        {
            if(i == 0)
            {
                normal = Vector2d(1, 0);
            }
            else if(i == 1)
            {
                normal = Vector2d(0, 1);
            }
            else if(i < shape1.getNumberOfNormals() + 2)
            {
                normal = shape1.getNormal(i);
            }
            else
            {
                normal = shape2.getNormal(i);
            }
            
            projection1 = shape1.getProjectionRange(normal);
            projection2 = shape2.getProjectionRange(normal);
            
            projectionIntersection = Check::rangeIntersection(projection1, projection2, inclusive);
            
            
            
            if(projectionIntersection.isOverlaping)
            {
                if(i == 0)
                {
                    penetration.x = projectionIntersection.value;
                }
                else if(i == 1)
                {
                    penetration.y = projectionIntersection.value;
                }
                
                if(!isset || projectionIntersection.value*projectionIntersection.value < mtv.magnatudeSquared())
                {
                    mtv = projectionIntersection.value * normal;
                    isset = true;
                }
            }
            else
            {
                return Result(false);
            }
        }
        
        return Result(true, mtv, penetration);
    }
    
    
    /////////////// Polygon collisions /////////////////
    
    // Polygon - Polygon
    
    template<typename T>
    Result test(const Polygon<T>& poly1, const Polygon<T>& poly2)
    {
        if(!poly1.isValid() || !poly2.isValid() || !poly1.isConvex || !poly2.isConvex)
        {
            return Result(false);
        }
        
        return getSATResult(poly1, poly2);
    }
    
    // Polygon - Line
    
    template<typename T>
    Result test(const Polygon<T>& poly, const Line<T>& line)
    {
        if(!poly.isValid() || !poly.isConvex)
        {
            return Result(false);
        }
        
        return getSATResult(poly, line);
    }
    template<typename T>
    Result test(const Line<T>& line, const Polygon<T>& poly){return -test(poly, line)};
    
    // Polygon - Rect
    
    template<typename T>
    Result test(const Polygon<T>& poly, const Rect<T>& rect)
    {
        if(!poly.isValid() || !poly.isConvex)
        {
            return Result(false);
        }
        
        return getSATResult(poly, rect);
    }
    template<typename T>
    Result test(const Rect<T>& rect, const Polygon<T>& poly){return -test(poly, rect)};
    
    
    // Polygon - Circle
    
    template<typename T>
    Result test(const Polygon<T>& poly, const Circle<T>& circle)
    {
        if(!poly.isValid() || !poly.isConvex)
        {
            return Result(false);
        }
        
        return getSATResult(poly, circle);
    }
    template<typename T>
    Result test(const Circle<T>& circle, const Polygon<T>& poly){return -test(poly, circle)};
    
    
    
    /////////////// Line collisions /////////////////
    
    // Line - Line
    
    template<typename T>
    Result test(const Line<T>& line1, const Line<T>& line2)
    {
        return getSATResult(line1, line2);
    }
    
    // Line - Rect
    
    template<typename T>
    Result test(const Line<T>& line, const Rect<T>& rect)
    {
        return getSATResult(line, rect);
    }
    template<typename T>
    Result test(const Rect<T>& rect, const Line<T>& line){return -test(line, rect)};
    
    
    // Line - Circle
    
    template<typename T>
    Result test(const Line<T>& line, const Circle<T>& circle)
    {
        return getSATResult(line, circle);
    }
    template<typename T>
    Result test(const Circle<T>& circle, const Line<T>& line){return -test(line, circle)};
    
    /////////////// Rect collisions /////////////////
    
    // Rect - Rect
    
    template<typename T>
    Result test(const Rect<T>& rect1, const Rect<T>& rect2, bool inclusive = false)
    {
        Check::Overlap xOverlap = Check::rangeIntersection(rect1.getHorizontal(), rect2.getHorizontal(), inclusive);
        Check::Overlap yOverlap = Check::rangeIntersection(rect1.getVertical(),   rect2.getVertical(),   inclusive);
        
        return Result
        (
         xOverlap.isOverlapping && yOverlap.isOverlapping,
         xOverlap.value < yOverlap.value ? Vector2d(xOverlap.value, 0) : Vector2d(0, yOverlap.value),
         Vector2d(xOverlap.value, yOverlap.value);
         );
    }
    
    // Rect - Circle
    
    template<typename T>
    Result test(const Rect<T>& rect, const Circle<T>& circle)
    {
        //return getSATResult(rect, circle);
    }
    template<typename T>
    Result test(const Circle<T>& circle, const Rect<T>& rect){return -test(rect, circle)};
    
    
    
    /////////////// Circle collisions /////////////////
    
    // Circle - Circle
    
    template<typename T>
    Result test(const Circle<T>& circle1, const Circle<T>& circle2, bool inclusive = false)
    {
        Vector2d centerToCenter = circle2.position - circle1.position;
        double maxDistance      = circle1.radius   + circle2.radius;
        double distance         = maxDistance - centerToCenter.magnatude();
        
        Vector2d penetration;
        penetration.x = Check::rangeIntersection(circle1.getRangeX(), circle2.getRangeX(), inclusive).getValue();
        penetration.y = Check::rangeIntersection(circle1.getRangeY(), circle2.getRangeY(), inclusive).getValue();
        
        if(inclusive)
        {
            if(distance >= 0)
            {
                return Result(true, centerToCenter.resize(distance), penetration)
            }
            return Result(false);
        }
        
        if(distance > 0)
        {
            return Result(true, centerToCenter.resize(distance), penetration)
        }
        return Result(false);
    }
}


// TODO: Add inclusive everywhere
// TODO: Rapair collisions with circles
// TODO: Add simple segments

#endif // COLLISIONS_V2_HPP_INCLUDED
