#include "probabilistic_roadmap.hpp"


Roadmap build_prm(int n_samples, int n_batch, int k_neighbors, const SearchSpace& search_space)
{
    Roadmap roadmap;    // Roadmap that we are going to produce

    // Get an initial sampling of the space to kick off the roadmap generation
    Eigen::MatrixXd initial_samples = search_space.sample_free_space(n_batch);

    for (int i=0; i < n_batch; i++) {
        Eigen::MatrixXd empty_neighbors(0, 0);
        Eigen::VectorXd empty_costs(0);

        roadmap.add_node(initial_samples.row(i), empty_neighbors, empty_costs);
    }

    // We're going to keep sampling from the free space until we have the number of nodes in the
    // roadmap that are desired.
    int node_count = n_batch;
    while (node_count < n_samples) {
        // Sample a batch from the free space
        Eigen::MatrixXd samples = search_space.sample_free_space(n_batch);

        // Loop through the samples in the batch
        for (int i=0; i < samples.rows(); i++) {
            Eigen::VectorXd sample = samples.row(i);

            // Get the k-nearest neighbors
            Eigen::MatrixXd cand_neighbors = roadmap.k_states_nearest(sample, k_neighbors);

            // We're going to populate two containers: a container of valid neighbors out of the k 
            // neighbors we got from the nearest neighbor search and a container of the costs of 
            // transitioning to these neighbors
            std::vector<Eigen::VectorXd> valid_neighbors;
            std::vector<double> transition_costs;
            for (int j=0; j < cand_neighbors.rows(); j++) {
                Eigen::VectorXd cand_neighbor = cand_neighbors.row(j);

                // If we have a valid transition between the sample and candidate neighbor (no collision)...
                if (search_space.valid_transition(sample, cand_neighbor)) {
                    // ... then add the node's state and transition cost to their respective 
                    // containers.
                    valid_neighbors.push_back(cand_neighbor);
                    transition_costs.push_back(search_space.transition_cost(sample, cand_neighbor));
                    
                    // Increment the node count
                    node_count++;

                    if (node_count >= n_samples)
                        break;
                }
            }

            // Put the data into a matrix for the neighbors and a vector for the costs
            int n_rows = valid_neighbors.size();
            int n_cols = search_space.state_size();

            Eigen::MatrixXd neighbors{n_rows, n_cols};
            Eigen::VectorXd costs{n_rows};
            
            for (int j=0; j < n_rows; j++) {
                neighbors.row(j) = valid_neighbors[j];
                costs(j) = transition_costs[j];
            }

            // Finally, add the sample to the roadmap with its neighbors and costs
            roadmap.add_node(sample, neighbors, costs);
        }
    }

    return roadmap;
}