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

struct RoadmapNode {
    Eigen::VectorXd state;

    Eigen::MatrixXd neighbors;
    Eigen::VectorXd costs;

    Eigen::VectorXd getState() const;
    Eigen::MatrixXd getNeighbors() const;
    Eigen::VectorXd getCosts() const;
};

/* Roadmap class */

typedef std::map<Eigen::VectorXd, RoadmapNode, std::function<bool(const Eigen::VectorXd&, const Eigen::VectorXd&)>> MapVectorXd;

class Roadmap {
    MapVectorXd roadmap;
    kdTree kdtree;

public:
    Roadmap() {
        roadmap = MapVectorXd {
            [](const Eigen::VectorXd& a, const Eigen::VectorXd& b)->bool
            {
                return std::lexicographical_compare(
                    a.data(), a.data() + a.size(),
                    b.data(), b.data() + b.size());
            }
        };
    };

    void add_node(const Eigen::VectorXd& state, const Eigen::MatrixXd& neighborStates, const Eigen::VectorXd& neighborCosts);

    RoadmapNode node_at(const Eigen::VectorXd& state) const;
    RoadmapNode node_nearest(const Eigen::VectorXd& state) const;
    std::vector<RoadmapNode> k_nodes_nearest(const Eigen::VectorXd& state, int k) const;

    Eigen::VectorXd state_nearest(const Eigen::VectorXd& state) const;
    Eigen::MatrixXd k_states_nearest(const Eigen::VectorXd& state, int k) const;

    std::vector<RoadmapNode> nodes() const;
};