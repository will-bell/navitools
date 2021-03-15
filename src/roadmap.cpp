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

void Roadmap::add_node(const Eigen::VectorXd& state, const Eigen::MatrixXd& neighborStates, 
    const Eigen::VectorXd& neighborCosts)
{
    roadmap[state] = {state, neighborStates, neighborCosts};

    if (neighborStates.rows()) {
        for (int i = 0; i < neighborStates.rows(); i++) {
            // We will catch the error if a state is not in the map using std::map::at()
            // Have to wrap this whole thing in the try/catch since we are getting a reference to
            // the node out of std::map::at().
            try {
                // Get references to the neighbors and costs for the neighbor node
                RoadmapNode& node = roadmap.at(neighborStates.row(i));
                Eigen::MatrixXd& neighbors = node.neighbors;
                
                Eigen::VectorXd& costs = node.costs;

                int nNeighbors = neighbors.rows();

                if (!nNeighbors) {
                    // Give the arrays the right size
                    neighbors.conservativeResize(1, state.size());
                    costs.conservativeResize(1, 1);

                    // Assign the neighbor and cost
                    neighbors.row(0) = state;
                    costs.row(0) = neighborCosts.row(i);
                }
                else {
                    // Append a neighbor to the node
                    neighbors.conservativeResize(nNeighbors + 1, Eigen::NoChange);
                    neighbors.row(nNeighbors) = state;
                    
                    // Append a cost to the node that corresponds to the new neighbor
                    costs.conservativeResize(nNeighbors + 1, Eigen::NoChange);
                    costs.row(nNeighbors) = neighborCosts.row(i);
                }
            }
            catch (const std::out_of_range& oor) {
                throw MissingStateRoadmapException{};
            }
        }
    }
}

std::vector<RoadmapNode> Roadmap::nodes() const
{
    std::vector<RoadmapNode> nodeVector;

    for (auto it = roadmap.begin(); it != roadmap.end(); ++it) {
        nodeVector.push_back(it->second);
    }

    return nodeVector;
}