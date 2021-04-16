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

double cross2d(const Eigen::Vector2d& u, const Eigen::Vector2d& v)
{
    return u.x() * v.y() - u.y() * v.x();
}

bool point_in_segment(const Eigen::Vector2d& point, const std::pair<Eigen::Vector2d, Eigen::Vector2d>& segment)
{
    double eps = std::numeric_limits<double>::epsilon();

    if (std::abs(segment.first.x() - segment.second.x()) > eps) {   // Segment is not vertical, test x coordinate
        if (segment.first.x() <= point.x() && point.x() <= segment.second.x())
            return true;
        if (segment.first.x() >= point.x() && point.x() >= segment.second.x())
            return true;
    }
    else {
        if (segment.first.y() <= point.y() && point.y() <= segment.second.y())   // Segment is vertical, test y coordinate
            return true;
        if (segment.first.y() >= point.y() && point.y() >= segment.second.y())
            return true;
    }

    return false;
}

int segments_intersect(const std::pair<Eigen::Vector2d, Eigen::Vector2d>& ab, const std::pair<Eigen::Vector2d, Eigen::Vector2d>& cd)
{
    double eps = std::numeric_limits<double>::epsilon();

    Eigen::Vector2d u = ab.second - ab.first;
    Eigen::Vector2d v = cd.second - cd.first;
    Eigen::Vector2d w = ab.first - cd.first;
    double D = cross2d(u, v);

    // Test if the segments are parallel
    if (std::abs(D) < eps) {
        double Du = std::abs(cross2d(u, w));
        double Dv = std::abs(cross2d(v, w));

        if (Du > eps || Dv > eps)
            return 0;   // Not colinear, therefore just parallel

        double du = u.dot(u);
        double dv = v.dot(v);

        if (du < eps && dv < eps) {
            Eigen::Vector2d diff = ab.first - cd.first;
            if (diff.dot(diff) < eps)
                return 0;
        }

        if (du < eps) {     // First segment degnerate
            if (point_in_segment(ab.first, cd)) 
                return 1;   // and in the second segment
            
            return 0;       // but not in the second segment
        }

        if (dv < eps) {     // Second segment degenerate
            if (point_in_segment(cd.first, ab))
                return 1;   // and in the first segment
            
            return 0;       // but not in the first segment
        }

        if (point_in_segment(ab.first, cd) || point_in_segment(ab.second, cd))
            return 2;       // Overlapping

        return 0;           // Colinear but not overlapping
    }

    // Segments are skew, so they may intersect at a single point
    // Check intersection along S1
    double t1 = cross2d(u, w) / D;
    if (t1 < 0 || t1 > 1)
        return 0;

    // Check intersection along S2
    double t2 = cross2d(v, w) / D;
    if (t2 < 0 || t2 > 1)
        return 0;

    return 1;
}

/*  Tests if a segment intersects a polygon

    Inputs:
        segment: a pair of points that form a finite line segment
        polygon: a valid polygon

    Returns:
        0 if the segment is wholly outside the polygon
        1 if the segment intersects at least facet of the polygon
        2 if the segment is overlapping a facet of the polygon
        3 if the segment is wholly contained by the polygon
*/
int segment_intersects_polygon(const std::pair<Eigen::Vector2d, Eigen::Vector2d>& segment, const Polygon& polygon)
{
    for (int i=0, j=1; i < polygon.n_points(); i++, j++) {
        std::pair<Eigen::Vector2d, Eigen::Vector2d> facet = {polygon[i], polygon[j]};

        int code = segments_intersect(segment, facet);
        if (code > 0)
            return code;
    }

    if (inside_polygon(segment.first, polygon)) {
        if (inside_polygon(segment.second, polygon))
            return 3;
    }

    return 0;
}