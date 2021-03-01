#include "kd_tree.hpp"
#include <iostream>
#include <numeric>


using vector2d = std::vector<const std::vector<double>*>;
using vector2diterator = vector2d::iterator;

using kdNode_ptr = std::shared_ptr<kdNode>;

kdNode_ptr build_kdTree(vector2diterator begin, vector2diterator end, int size, int depth)
{
    if (!std::distance(begin, end))
        return kdNode_ptr();

    int axis = depth % size;

    // Sort the slice
    std::sort(begin, end, 
        [axis](const std::vector<double>* a, const std::vector<double>* b) 
        { 
            return (*a)[axis] < (*b)[axis];
        });

    // Get the median
    int median = std::distance(begin, end) / 2;

    // Recurse
    kdNode_ptr left = build_kdTree(begin, begin + median, size, depth + 1);
    kdNode_ptr right = build_kdTree(begin + median + 1, end, size, depth + 1);

    kdNode_ptr root = std::make_shared<kdNode>();

    root->left = left;
    root->right = right;
    root->state = **(begin + median);

    return root;
}

kdNode_ptr build_kdTree(vector2d& states)
{
    // All states should have the same size
    int size = states[0]->size();

    // Start with the first dimension
    int axis = 0;

    // Sort the points
    std::sort(states.begin(), states.end(), 
        [axis](const std::vector<double>* a, const std::vector<double>* b)
        { 
            return (*a)[axis] < (*b)[axis];
        });

    // Get the median
    int median = states.size() / 2;

    // Recurse
    kdNode_ptr left = build_kdTree(states.begin(), states.begin() + median, size, 1);
    kdNode_ptr right = build_kdTree(states.begin() + median + 1, states.end(), size, 1);

    kdNode_ptr root = std::make_shared<kdNode>();

    root->left = left;
    root->right = right;
    root->state = **(states.begin() + median);

    return root;
}

void kdTree_append_state(const std::vector<double>& state, kdNode_ptr root, int depth = 0)
{
    int axis = depth % state.size();

    if (state[axis] < root->state[axis]) {
        // Look left
        if (root->left) {
            // If there is a left node, proceed in that direction
            kdTree_append_state(state, root->left, depth + 1);
        }
        else {
            // Else, this is the location for the point in the tree
            kdNode_ptr newNode = std::make_shared<kdNode>();
            newNode->state = state;
            
            root->left = newNode;
        }
    }
    else {
        // Look right
        if (root->right) {
            // If there is a right node, proceed in that direction
            kdTree_append_state(state, root->right, depth + 1);
        }
        else {
            // Else, this is the location for the point in the tree
            kdNode_ptr newNode = std::make_shared<kdNode>();
            newNode->state = state;
            
            root->right = newNode;
        }
    }
}

std::vector<kdNode_ptr> kdTree_at_depth(kdNode_ptr kd_tree, std::vector<kdNode_ptr>& nodes, int desired_depth, int current_depth)
{
    if (desired_depth == current_depth) {
        nodes.push_back(kd_tree);
    }
    else {
        if (kd_tree->left) {
            kdTree_at_depth(kd_tree->left, nodes, desired_depth, current_depth + 1);
        }
        if (kd_tree->right) {
            kdTree_at_depth(kd_tree->right, nodes, desired_depth, current_depth + 1);
        }
    }

    return nodes;
}

std::vector<kdNode_ptr> kdTree_at_depth(kdNode_ptr kd_tree, int desired_depth)
{
    std::vector<kdNode_ptr> nodes;

    int current_depth = 0;

    if (desired_depth == current_depth) {
        nodes.push_back(kd_tree);
    }
    else {
        if (kd_tree->left) {
            kdTree_at_depth(kd_tree->left, nodes, desired_depth, current_depth + 1);
        }
        if (kd_tree->right) {
            kdTree_at_depth(kd_tree->right, nodes, desired_depth, current_depth + 1);
        }
    }

    return nodes;
}

double square_distance(const std::vector<double> u, const std::vector<double> v)
{
    if (u.size() != v.size()) {
        // Raise exception: vectors must have the same size
    }

    double mag = 0.;

    for (int i = 0; i < u.size(); ++i) {
        double d = u[i] - v[i];
        mag += std::pow(d, 2);
    }

    return mag;
}

void kdTree_nearest_neighbor(kdNode_ptr root, const std::vector<double>& search_state, int depth, double& best_distance, kdNode_ptr& best_node)
{
    int axis = depth % search_state.size();

    double dist = square_distance(search_state, root->state);
    if (dist < best_distance) {
        best_distance = dist;
        best_node = root;
    }

    if (search_state[axis] < root->state[axis] && root->left) {
        kdTree_nearest_neighbor(root->left, search_state, depth + 1, best_distance, best_node);

        if (std::pow(std::abs(search_state[axis] - root->state[axis]), 2) < best_distance && root->right) {
            kdTree_nearest_neighbor(root->right, search_state, depth + 1, best_distance, best_node);
        }
    }
    else if(search_state[axis] > root->state[axis] && root->right) {
        kdTree_nearest_neighbor(root->right, search_state, depth + 1, best_distance, best_node);

        if (std::pow(std::abs(search_state[axis] - root->state[axis]), 2) < best_distance && root->left) {
            kdTree_nearest_neighbor(root->left, search_state, depth + 1, best_distance, best_node);
        }
    }
}

std::vector<double> kdTree_nearest_neighbor(kdNode_ptr kd_tree, const std::vector<double>& search_state)
{
    double best_distance = std::numeric_limits<double>::max();
    kdNode_ptr best_node = nullptr;

    kdTree_nearest_neighbor(kd_tree, search_state, 0, best_distance, best_node);

    return best_node->state;
}

void kdTree::append_state(const std::vector<double>& state)
{
    kdTree_append_state(state, root);
}


/* CLASS IMPLEMENTATION */

kdTree::kdTree(const std::vector<std::vector<double>>& states)
{
    dim = states[0].size();

    std::vector<const std::vector<double>*> state_ptrs;
        
    for (int i = 0; i < states.size(); i++) {
        state_ptrs.push_back(&(states[i]));
    }

    root = build_kdTree(state_ptrs);
}

kdTree::kdTree(std::vector<const std::vector<double>*> states)
{
    dim = states[0]->size();

    root = build_kdTree(states);
}

std::vector<std::vector<double>> kdTree::at_depth(int depth) const
{
    std::vector<kdNode_ptr> nodes = kdTree_at_depth(root, depth);

    std::vector<std::vector<double>> states;
    for (auto node : nodes) {
        states.push_back(node->state);
    }

    return states;
}

std::vector<double> kdTree::nearest_neighbor(const std::vector<double>& search_state) const
{
    return kdTree_nearest_neighbor(root, search_state);
}

int kdTree::count_states() const
{
    int depth = 0;
    std::vector<std::vector<double>> this_level = at_depth(depth);

    int n_states = 0;
    while (this_level.size()) {
        n_states += this_level.size();
        
        depth++;
        this_level = at_depth(depth);
    }

    return n_states;
}