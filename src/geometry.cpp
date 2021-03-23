#include "geometry.hpp"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <numeric>

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

/*  Tests the orientation of a triangle
    Input: three points p0, p1, p2 that define a triangle
    Returns: 
        >0 if the orientation is counter-clockwise
        =0 if the points all lie on a line
        <0 if the orientation is clockwise
*/
int triangle_orientation(const Eigen::Vector2d& p0, const Eigen::Vector2d& p1, const Eigen::Vector2d& p2)
{
    double val = is_left(p0, p1, p2);
    
    if (val < -std::numeric_limits<double>::epsilon())
        return -1;

    if (val > -std::numeric_limits<double>::epsilon())
        return 1;
    
    return 0;
}

/*  Tests the orientation of a polygon
    Input: the points that make up the polygon
    Returns: 
        >0 if the orientation is counter-clockwise
        =0 if the points all lie on a line
        <0 if the orientation is clockwise
*/
int polygon_orientation(const Eigen::MatrixX2d& points)
{
    double rmin = 0;
    double xmin = points.row(0).x();
    double ymin = points.row(0).y();

    for (int i=0; i<points.rows(); i++) {
        if (points.row(i).y() > ymin)
            continue;
        if (points.row(i).y() == ymin) {
            if (points.row(i).x() < xmin)
                continue;
        }
        rmin = i;
        xmin = points.row(i).x();
        ymin = points.row(i).y();
    }

    if (rmin == 0)
        return triangle_orientation(points.row(points.rows() - 1), points.row(0), points.row(1));
    
    return triangle_orientation(points.row(rmin - 1), points.row(rmin), points.row(rmin + 1));
}

Polygon::Polygon(Eigen::MatrixX2d points)
{
    _n_points = points.rows();
    if (_n_points < 3)
        throw(1);   // Invalid polygon
    
    _orientation = polygon_orientation(points);
    if (_orientation == 0)
        throw(2);   // Invalid polygon

    _points = points;
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

    if (polygon.is_solid())
        return wn != 0;

    return wn == 0;
}

bool inside_any_polygon(const Eigen::Vector2d& point, const std::vector<Polygon>& polygons)
{
    for (auto polygon : polygons) {
        if (inside_polygon(point, polygon))
            return true;
    }
    return false;
}