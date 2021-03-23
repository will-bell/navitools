#pragma once

#include <vector>
#include <Eigen/Core>

class Polygon {
    Eigen::MatrixX2d _points;
    int _n_points;
    int _orientation;

public:
    Polygon(Eigen::MatrixX2d points);

    Eigen::Vector2d operator[](int i) const {
        // Wrap indices to the number of vertices in the polygon
        int j = (_n_points + (i % _n_points)) % _n_points;
        return _points.row(j);
    }

    Eigen::MatrixX2d points() const {
        return _points;
    }

    int n_points() const {
        return _n_points;
    }

    /*  Orientation of the polygon
        Returns: 
            >0 if orientation is counter-clockwise
            <0 if orientation is clockwise
    */
    int orientation() const {
        return _orientation;
    }

    bool is_solid() const {
        return _orientation > 0;
    }

    bool is_hollow() const {
        return _orientation < 0;
    }
};

bool inside_polygon(const Eigen::Vector2d& point, const Polygon& polygon);
