#include "geometry.hpp"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

Polygon::Polygon(Eigen::MatrixX2d points)
{
    _points = points;
    _n_points = points.rows();
}

/*  Tests if a point is left|on|right of an infinite line
    Input: three points p0, p1, p2
    Returns: 
        >0 if p2 is left of line through p0 and p1
        =0 if p2 is on the line
        <0 if p2 is right of the line
*/
double is_left(const Eigen::Vector2d& p0, const Eigen::Vector2d& p1, const Eigen::Vector2d& p2)
{
    return (p1.x() - p0.x()) * (p2.y() - p0.y()) - (p2.x() - p0.x()) * (p1.y() - p0.y());
}

bool inside_polygon(const Eigen::Vector2d& point, const Polygon& polygon)
{
    int wn = 0;

    for (int i = 0; i < polygon.n_points(); i++) {       // edge from vertex[i] to vertex[i+1]
        if (polygon[i].y() <= point.y()) {                          // point above vertex[i]
            if (polygon[i+1].y() > point.y())                       // upward crossing
                if (is_left(polygon[i], polygon[i+1], point) > 0)   // point left of edge
                    wn++;                                           // valid up intersect
        }
        else {                                                      // point below vertex[i]
            if (polygon[i+1].y() <= point.y())                      // downward crossing
                if (is_left(polygon[i], polygon[i+1], point) <= 0)  // point right of edge
                    wn--;                                           // valid down intersect
        }
    }

    return wn != 0;
}