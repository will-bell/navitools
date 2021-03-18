#pragma once

#include <vector>
#include <Eigen/Core>

class Polygon {
    Eigen::MatrixX2d _points;
    int _n_points;

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
};

bool inside_polygon(const Eigen::Vector2d& point, const Polygon& polygon);
