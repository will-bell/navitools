#pragma once

#include <queue>
#include "roadmap.hpp"

struct pairCompare {
    bool operator()(const std::pair<Eigen::VectorXd, double>& a, const std::pair<Eigen::VectorXd, double>& b) {
        return a.second > b.second;
    }
};

typedef std::priority_queue<std::pair<Eigen::VectorXd, double>, std::vector<std::pair<Eigen::VectorXd, double>>, pairCompare> MinPairQueue;

Eigen::MatrixXd dijkstra(const Roadmap& graph, const Eigen::VectorXd& start_state, const Eigen::VectorXd& goal_state);

// Eigen::MatrixXd aStar(const Roadmap& graph, const Eigen::VectorXd& start_state, const Eigen::VectorXd& goal_state, 
//     double heuristic(const Eigen::VectorXd&, const Eigen::VectorXd&));
