#pragma once

#include <random>
#include <Eigen/Core>
#include "geometry.hpp"

#include <iostream>

class SearchSpace {
    public:
        virtual Eigen::MatrixXd sample_free_space(int n) {
            return {1, 1};
        }

        virtual bool straight_line_collision(const Eigen::VectorXd& a, const Eigen::VectorXd& b) {
            return true;
        }
};

class PolygonSpace : SearchSpace {
    std::vector<Polygon> _polygons;
    
    std::tuple<double, double> _xrange, _yrange;
    
    std::uniform_real_distribution<double> _xuniform, _yuniform;

    std::mt19937 _xgen, _ygen;

    double _sample_x() {return _xuniform(_xgen);}

    double _sample_y() {return _yuniform(_ygen);}

    public:
        PolygonSpace(const std::vector<Polygon>& polygons, std::pair<double, double> xrange, std::pair<double, double> yrange) {
            _polygons = polygons;
            
            _xrange = xrange;
            _yrange = yrange;
            
            std::random_device xrd;
            std::random_device yrd;

            _xgen = std::mt19937{xrd()};
            _ygen = std::mt19937{yrd()};

            _xuniform = std::uniform_real_distribution<double>{xrange.first, xrange.second};
            _yuniform = std::uniform_real_distribution<double>{yrange.first, yrange.second};
        }

        Eigen::MatrixXd sample_free_space(int n);

        bool straight_line_collision(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
};
