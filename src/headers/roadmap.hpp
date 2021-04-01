#pragma once

#include <vector>
#include <map>
#include <Eigen/Core>
#include "kd_tree.hpp"

/* Custom exceptions for the roadmap */

struct MissingStateRoadmapException : public std::exception
{
    const char* what() const throw()
    {
        return "Given state is missing from the Roadmap";
    }
};

/* Roadmap Node class */

class RoadmapNode {
    Eigen::VectorXd _state;
    Eigen::MatrixXd _neighbors;
    Eigen::VectorXd _costs;

    // Setters are private
    void set_state(const Eigen::VectorXd& value) {_state = value;}
    void set_neighbors(const Eigen::MatrixXd& value) {_neighbors = value;}
    void set_costs(const Eigen::VectorXd& value) {_costs = value;}

    // Give Roadmap access to the node's private members
    friend class Roadmap;

public:
    RoadmapNode() {};
    RoadmapNode(const Eigen::VectorXd& state, const Eigen::MatrixXd neighbors, const Eigen::VectorXd& costs) : _state(state), _neighbors(neighbors), _costs(costs) {}

    // Getters
    Eigen::VectorXd get_state() const {return _state;}
    Eigen::MatrixXd get_neighbors() const {return _neighbors;}
    Eigen::VectorXd get_costs() const {return _costs;}
};

/* Roadmap class */

typedef std::map<Eigen::VectorXd, RoadmapNode, std::function<bool(const Eigen::VectorXd&, const Eigen::VectorXd&)>> MapVectorXd;

#define MAPVECTORXD MapVectorXd {                                           \
            [](const Eigen::VectorXd& a, const Eigen::VectorXd& b)->bool    \
            {                                                               \
                return std::lexicographical_compare(                        \
                    a.data(), a.data() + a.size(),                          \
                    b.data(), b.data() + b.size());                         \
            }                                                               \
        }                                                                   \

class Roadmap {
    MapVectorXd roadmap;
    kdTree kdtree;

    int _state_size = -1;
    int _n_states = 0;

    // Setters are private
    void set_state_size(int value) {_state_size = value;}
    void increment_n_states() {_n_states++;}

public:
    Roadmap() {
        roadmap = MAPVECTORXD;
        set_state_size(-1);
    }

    Roadmap(int state_size) {
        roadmap = MAPVECTORXD;
        set_state_size(state_size);
    };

    void add_node(const Eigen::VectorXd& state, const Eigen::MatrixXd& neighborStates, const Eigen::VectorXd& neighborCosts);

    RoadmapNode node_at(const Eigen::VectorXd& state) const;
    RoadmapNode node_nearest(const Eigen::VectorXd& state) const;
    std::vector<RoadmapNode> k_nodes_nearest(const Eigen::VectorXd& state, int k) const;

    Eigen::VectorXd state_nearest(const Eigen::VectorXd& state) const;
    Eigen::MatrixXd k_states_nearest(const Eigen::VectorXd& state, int k) const;

    // Getters
    int get_state_size() const {return _state_size;}
    int get_n_states() const {return _n_states;}
    std::vector<RoadmapNode> get_nodes() const;
    Eigen::MatrixXd get_states() const;
};