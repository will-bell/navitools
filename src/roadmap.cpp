#include <stdexcept>
#include "roadmap.hpp"
#include "exceptions.hpp"

Eigen::VectorXd RoadmapNode::getState() const
{
    return state;
}

Eigen::MatrixXd RoadmapNode::getNeighbors() const
{
    return neighbors;
}

Eigen::VectorXd RoadmapNode::getCosts() const
{
    return costs;
}

using MatrixXd_ptr = std::unique_ptr<Eigen::MatrixXd>;
using VectorXd_ptr = std::unique_ptr<Eigen::VectorXd>;

void Roadmap::add_node(const Eigen::VectorXd& state, const Eigen::MatrixXd& neighborStates, 
    const Eigen::VectorXd& neighborCosts)
{
    if (neighborStates.rows()) {
        std::vector<RoadmapNode*> nodes_to_modify;
        std::vector<MatrixXd_ptr> extended_neighbors_lists;
        std::vector<VectorXd_ptr> extended_costs_lists;

        for (int i=0; i < neighborStates.rows(); i++) {
            try {
                RoadmapNode& node = roadmap.at(neighborStates.row(i));
                
                nodes_to_modify.push_back(&node);

                MatrixXd_ptr extended_neighbors_list = std::make_unique<Eigen::MatrixXd>(node.neighbors);
                VectorXd_ptr extended_costs_list = std::make_unique<Eigen::VectorXd>(node.costs);

                int n_neighbors = extended_neighbors_list->rows();
                if (!n_neighbors) {
                    // Give the arrays the right size
                    extended_neighbors_list->conservativeResize(1, state.size());
                    extended_costs_list->conservativeResize(1, 1);

                    // Assign the neighbor and cost
                    extended_neighbors_list->row(0) = state;
                    extended_costs_list->row(0) = neighborCosts.row(i);
                }
                else {
                    // Append a neighbor to the node
                    extended_neighbors_list->conservativeResize(n_neighbors + 1, Eigen::NoChange);
                    extended_neighbors_list->row(n_neighbors) = state;
                    
                    // Append a cost to the node that corresponds to the new neighbor
                    extended_costs_list->conservativeResize(n_neighbors + 1, Eigen::NoChange);
                    extended_costs_list->row(n_neighbors) = neighborCosts.row(i);
                }

                extended_neighbors_lists.push_back(std::move(extended_neighbors_list));
                extended_costs_lists.push_back(std::move(extended_costs_list));
            }
            catch (const std::out_of_range& oor) {
                throw MissingStateRoadmapException{};
            }
        }

        // Got through accessing all the states with no errors, so we can now connect them
        for (int i=0; i < neighborStates.rows(); i++) {
            RoadmapNode* node = nodes_to_modify[i];
            node->neighbors = *(extended_neighbors_lists[i]);
            node->costs = *(extended_costs_lists[i]);
        }
    }
    // Add the node to the map
    roadmap[state] = {state, neighborStates, neighborCosts};

    // Add the state to the k-d tree for quick searching
    kdtree.append_state(state);
}

std::vector<RoadmapNode> Roadmap::nodes() const
{
    std::vector<RoadmapNode> nodeVector;

    for (auto it = roadmap.begin(); it != roadmap.end(); ++it)
        nodeVector.push_back(it->second);

    return nodeVector;
}

RoadmapNode Roadmap::node_at(const Eigen::VectorXd& state) const 
{
    RoadmapNode node;
    try {
        node = roadmap.at(state);
    }
    catch (const std::out_of_range& oor) {
        throw MissingStateRoadmapException{};
    }

    return node;
}

RoadmapNode Roadmap::node_nearest(const Eigen::VectorXd& state) const 
{
    Eigen::VectorXd nearest_state = kdtree.nearest_neighbor(state);

    // We can access the node without worrying about catching errors since we know this state is in
    // the map.
    RoadmapNode node = roadmap.at(nearest_state);

    return node;
}

std::vector<RoadmapNode> Roadmap::k_nodes_nearest(const Eigen::VectorXd& state, int k) const
{
    Eigen::MatrixXd k_nearest_states = kdtree.k_nearest_neighbors(state, k);

    std::vector<RoadmapNode> nodes;
    for (int i = 0; i < k_nearest_states.rows(); i++)
        // We can access the nodes without worrying about catching errors since we know these states
        // are in the map.
        nodes.push_back(roadmap.at(k_nearest_states.row(i)));

    return nodes;
}

Eigen::VectorXd Roadmap::state_nearest(const Eigen::VectorXd& state) const
{
    return kdtree.nearest_neighbor(state);
}

Eigen::MatrixXd Roadmap::k_states_nearest(const Eigen::VectorXd& state, int k) const
{
    return kdtree.k_nearest_neighbors(state, k);
}