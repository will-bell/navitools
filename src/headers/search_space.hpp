#pragma once

#include <random>
#include <Eigen/Core>
#include "geometry.hpp"


class SearchSpace {
    int _state_size;

    public:
        SearchSpace() {}

        SearchSpace(int state_size) {
            set_state_size(state_size);
        }

        virtual Eigen::MatrixXd sample_free_space(int n) const {
            return {1, 1};
        }

        virtual bool valid_transition(const Eigen::VectorXd& a, const Eigen::VectorXd& b) const {
            return true;
        }

        virtual double transition_cost(const Eigen::VectorXd& a, const Eigen::VectorXd& b) const{
            return 0.;
        }

        int state_size() const {return _state_size;}

    protected:
        void set_state_size(int state_size) {_state_size = state_size;}
};


template<typename Numeric, typename Generator = std::mt19937>
Numeric random(Numeric from, Numeric to)
{
    thread_local static Generator gen(std::random_device{}());

    using dist_type = typename std::conditional
    <
        std::is_integral<Numeric>::value
        , std::uniform_int_distribution<Numeric>
        , std::uniform_real_distribution<Numeric>
    >::type;

    thread_local static dist_type dist;

    return dist(gen, typename dist_type::param_type{from, to});
}


class PolygonSpace : public SearchSpace {
    std::vector<Polygon> _polygons;
    
    std::pair<double, double> _xrange, _yrange;

    double _sample_x() const {
        return random<double>(_xrange.first, _xrange.second);
    }

    double _sample_y() const {
        return random<double>(_yrange.first, _yrange.second);
    }

    public:
        PolygonSpace(const std::vector<Polygon>& polygons, std::pair<double, double> xrange, std::pair<double, double> yrange) {
            _polygons = polygons;
            
            _xrange = xrange;
            _yrange = yrange;

            set_state_size(2);
        }

        Eigen::MatrixXd sample_free_space(int n) const;

        bool valid_transition(const Eigen::VectorXd& a, const Eigen::VectorXd& b) const;

        double transition_cost(const Eigen::VectorXd& a, const Eigen::VectorXd& b) const;
};
