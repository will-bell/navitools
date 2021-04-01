#include <stdexcept>
#include "roadmap.hpp"
#include "exceptions.hpp"

using MatrixXd_ptr = std::unique_ptr<Eigen::MatrixXd>;
using VectorXd_ptr = std::unique_ptr<Eigen::VectorXd>;

void Roadmap::add_node(const Eigen::VectorXd& state, const Eigen::MatrixXd& neighborStates, 
    const Eigen::VectorXd& neighborCosts)
{
    if (get_state_size() < 0) {
        set_state_size(state.size());
    }
    else if (state.size() != get_state_size()) {
        throw BadStateSizeException{"Given state does not have the same size as the Roadmap's state space"};
    }

    if (neighborStates.rows()) {
        std::vector<RoadmapNode*> nodes_to_modify;
        std::vector<MatrixXd_ptr> extended_neighbors_lists;
        std::vector<VectorXd_ptr> extended_costs_lists;

        for (int i=0; i < neighborStates.rows(); i++) {
            if (neighborStates.row(i).size() != get_state_size())
                throw BadStateSizeException{"Given neighbor state does not have the same size as the Roadmap's state space"};

            try {
                RoadmapNode& node = roadmap.at(neighborStates.row(i));
                
                nodes_to_modify.push_back(&node);

                MatrixXd_ptr extended_neighbors_list = std::make_unique<Eigen::MatrixXd>(node._neighbors);
                VectorXd_ptr extended_costs_list = std::make_unique<Eigen::VectorXd>(node._costs);

                int n_neighbors = extended_neighbors_list->rows();
                if (!n_neighbors) {
                    // Give the arrays the right size
                    extended_neighbors_list->conservativeResize(1, get_state_size());
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

            node->set_neighbors(*(extended_neighbors_lists[i]));
            node->set_costs(*(extended_costs_lists[i]));
        }
    }
    // Add the node to the map
    roadmap[state] = {state, neighborStates, neighborCosts};

    increment_n_states();

    // Add the state to the k-d tree for quick searching
    kdtree.append_state(state);  
}

std::vector<RoadmapNode> Roadmap::get_nodes() const
{
    std::vector<RoadmapNode> nodeVector;

    for (auto it = roadmap.begin(); it != roadmap.end(); ++it)
        nodeVector.push_back(it->second);

    return nodeVector;
}

Eigen::MatrixXd Roadmap::get_states() const
{
    Eigen::MatrixXd states(get_n_states(), get_state_size());

    int i = 0; 
    for (auto it = roadmap.begin(); it != roadmap.end(); ++it, ++i)
        states.row(i) = it->first;

    return states;
}

RoadmapNode Roadmap::node_at(const Eigen::VectorXd& state) const 
{
    try {
        RoadmapNode node = roadmap.at(state);
        
        return node;
    }
    catch (const std::out_of_range& oor) {
        throw MissingStateRoadmapException{};
    }
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