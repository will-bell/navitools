#include <queue>
#include <unordered_map>
#include "search_roadmap.hpp"

Eigen::MatrixXd reconstruct_path(MapVectorXd<Eigen::VectorXd>& back_pointer, const Eigen::VectorXd& start_state, const Eigen::VectorXd& goal_state)
{
    Eigen::VectorXd current_state = goal_state;
    std::vector<Eigen::VectorXd> path_reverse;

    while (current_state != start_state) {
        path_reverse.push_back(current_state);
        current_state = back_pointer[current_state];
    }

    path_reverse.push_back(start_state);

    Eigen::MatrixXd path(start_state.size(), path_reverse.size());
    for (int i=path_reverse.size()-1, j=0; i >= 0; i--, j++)
        path.row(j) = path_reverse[i];

    return path;
}

Eigen::MatrixXd dijkstra(const Roadmap& roadmap, const Eigen::VectorXd& start_state, const Eigen::VectorXd& goal_state)
{
    MapVectorXd<double> cost_so_far = MAPVECTORXD(double);
    MapVectorXd<Eigen::VectorXd> back_pointer = MAPVECTORXD(Eigen::VectorXd);

    cost_so_far[start_state] = 0.;

    MinPairQueue queue;
    queue.push({start_state, 0.});

    while (!queue.empty()) {
        std::pair<Eigen::VectorXd, double> current = queue.top();
        queue.pop();

        // Information about the current node
        Eigen::VectorXd current_state = current.first;
        double path_cost = current.second;

        if (current_state == goal_state)
            return reconstruct_path(back_pointer, start_state, goal_state);

        Eigen::MatrixXd neighbors = roadmap.node_at(current_state).get_neighbors();
        Eigen::VectorXd costs = roadmap.node_at(current_state).get_costs();
        for (int i=0; i < neighbors.rows(); i++) {
            // Information about the neighbor
            Eigen::VectorXd neighbor_state = neighbors.row(i);
            double edge_cost = costs(i);

            // New path cost to be evaluated
            double new_cost = path_cost + edge_cost;

            // If the next path cost is better than the current recorded, update
            if (new_cost < cost_so_far[neighbor_state]) {
                // Update the maps
                cost_so_far[neighbor_state] = new_cost;
                back_pointer[neighbor_state] = current_state;

                // Add the expanded node to the queue
                queue.push({neighbor_state, new_cost});
            }
        }
    }

    return {};
}