#include <algorithm>
#include <numeric>
#include <exception>
#include "kd_tree.hpp"
#include "exceptions.hpp"

using kdNode_ptr = std::shared_ptr<kdNode>;

/* Algorithm Implementation */

kdNode_ptr build_kdTree(std::vector<Eigen::VectorXd>::iterator begin, std::vector<Eigen::VectorXd>::iterator end, 
    int size, int depth)
{
    if (!std::distance(begin, end))
        return kdNode_ptr();

    int axis = depth % size;

    // Sort the slice
    std::sort(begin, end, 
        [axis](const Eigen::VectorXd& a, const Eigen::VectorXd& b) 
        { 
            return a[axis] < b[axis];
        });

    // Get the median
    int median = std::distance(begin, end) / 2;

    // Recurse
    kdNode_ptr left = build_kdTree(begin, begin + median, size, depth + 1);
    kdNode_ptr right = build_kdTree(begin + median + 1, end, size, depth + 1);

    kdNode_ptr root = std::make_shared<kdNode>();

    root->left = left;
    root->right = right;
    root->state = *(begin + median);

    return root;
}

kdNode_ptr build_kdTree(std::vector<Eigen::VectorXd>& states, int size)
{
    // Start with the first dimension
    int axis = 0;

    // Sort the points
    std::sort(states.begin(), states.end(), 
        [axis](const Eigen::VectorXd& a, const Eigen::VectorXd& b)
        { 
            return a[axis] < b[axis];
        });

    // Get the median
    int median = states.size() / 2;

    // Recurse
    kdNode_ptr left = build_kdTree(states.begin(), states.begin() + median, size, 1);
    kdNode_ptr right = build_kdTree(states.begin() + median + 1, states.end(), size, 1);

    kdNode_ptr root = std::make_shared<kdNode>();

    root->left = left;
    root->right = right;
    root->state = *(states.begin() + median);

    return root;
}

void kdTree_append_state(const Eigen::VectorXd& state, kdNode_ptr root, int depth = 0)
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

double square_distance(const Eigen::VectorXd& u, const Eigen::VectorXd& v)
{
    Eigen::VectorXd diff = u - v;

    return diff.dot(diff);
}

void kdTree_nearest_neighbor(kdNode_ptr root, const Eigen::VectorXd& search_state, int depth, double& best_distance, kdNode_ptr& best_node)
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

Eigen::VectorXd kdTree_nearest_neighbor(kdNode_ptr kd_tree, const Eigen::VectorXd& search_state)
{
    double best_distance = std::numeric_limits<double>::max();
    kdNode_ptr best_node = nullptr;

    kdTree_nearest_neighbor(kd_tree, search_state, 0, best_distance, best_node);

    return best_node->state;
}

struct kNearest {
    kNearest(int k) {
        _k = k;

        for (int i = 0; i < k; i++) {
            dist_list.push_back(std::numeric_limits<double>::max());
            node_list.push_back(nullptr);
        }
    }

    int _k;
    int len;
    std::vector<double> dist_list;
    std::vector<kdNode_ptr> node_list;

    void insert(double value, kdNode_ptr node) {
        std::vector<double>::iterator dit = dist_list.begin();
        std::vector<kdNode_ptr>::iterator nit = node_list.begin();

        for (int i = 0; i < _k; i++) {
            if (value < dist_list[i]) {
                dist_list.insert(dit + i, value);
                dist_list.pop_back();

                node_list.insert(nit + i, node);
                node_list.pop_back();

                break;
            }
        }
    }

    double bottom_distance() {
        return dist_list.back();
    }

    kdNode_ptr bottom_node() {
        return node_list.back();
    }

    Eigen::MatrixXd states() const {
        // Assuming all states have the same size
        Eigen::MatrixXd _states(_k, node_list[0]->state.size());

        for (int i = 0; i < _k; i++)
            _states.row(i) = node_list[i]->state;

        return _states;
    }  
};

void kdTree_k_nearest_neighbors(kdNode_ptr root, const Eigen::VectorXd& search_state, int depth, kNearest& nearest)
{
    int axis = depth % search_state.size();

    double dist = square_distance(search_state, root->state);

    if (dist < nearest.bottom_distance()) {
        nearest.insert(dist, root);
    }

    if (search_state[axis] < root->state[axis]) {
        if (root->left) {
            kdTree_k_nearest_neighbors(root->left, search_state, depth + 1, nearest);

            if (std::pow(search_state[axis] - root->state[axis], 2) < nearest.bottom_distance() && root->right) {
                kdTree_k_nearest_neighbors(root->right, search_state, depth + 1, nearest);
            }
        }
        else if (root->right) {
            kdTree_k_nearest_neighbors(root->right, search_state, depth + 1, nearest);
        }
    }
    else if(search_state[axis] > root->state[axis]) {
        if (root->right) {
            kdTree_k_nearest_neighbors(root->right, search_state, depth + 1, nearest);

            if (std::pow(search_state[axis] - root->state[axis], 2) < nearest.bottom_distance() && root->left) {
                kdTree_k_nearest_neighbors(root->left, search_state, depth + 1, nearest);
            }
        }
        else if (root->left) {
            kdTree_k_nearest_neighbors(root->left, search_state, depth + 1, nearest);
        }
    }
}

Eigen::MatrixXd kdTree_k_nearest_neighbors(kdNode_ptr kd_tree, const Eigen::VectorXd& search_state, int k)
{
    kNearest nearest = kNearest{k};

    kdTree_k_nearest_neighbors(kd_tree, search_state, 0, nearest);

    return nearest.states();
}

/* CLASS IMPLEMENTATION */

kdTree::kdTree(const std::vector<Eigen::VectorXd>& states)
{
    state_size = states[0].size();
    std::vector<Eigen::VectorXd> copy_states;

    for (auto state : states) {
        if (state.size() != state_size) {
            throw VaryingStateSizeException{};
        }
        copy_states.push_back(state);
    }

    root = build_kdTree(copy_states, state_size);
}

kdTree::kdTree(const Eigen::MatrixXd& states)
{
    if (states.rows()) {
        state_size = states.cols();

        std::vector<Eigen::VectorXd> copy_states;

        for (int i = 0; i < states.rows(); i++) {
            copy_states.push_back(states.row(i));
        }

        root = build_kdTree(copy_states, state_size);
    }
}

void kdTree::append_state(const Eigen::VectorXd& state)
{
    if (root) {
        if (state.size() != state_size)
            throw BadStateSizeException{};

        kdTree_append_state(state, root);
    }
    else {
        state_size = state.size();

        root = std::make_shared<kdNode>();
        root->state = state;
    }
}

Eigen::MatrixXd kdTree::at_depth(int depth) const
{
    if (root) {
        std::vector<kdNode_ptr> nodes = kdTree_at_depth(root, depth);

        std::vector<Eigen::VectorXd> states;
        for (auto node : nodes) {
            states.push_back(node->state);
        }

        int n_states = states.size();

        Eigen::MatrixXd stacked_states(n_states, state_size);
        
        for (int i = 0; i < n_states; i++)
            stacked_states.row(i) = states[i];

        return stacked_states;
    }
    else {
        Eigen::MatrixXd empty(0, 0);

        return empty;
    }
}

Eigen::VectorXd kdTree::nearest_neighbor(const Eigen::VectorXd& search_state) const
{
    if (root) {
        if (search_state.size() != state_size) {
            throw BadStateSizeException{};
        }

        return kdTree_nearest_neighbor(root, search_state);
    }
    else {
        throw EmptyTreeException{};
    }
}

Eigen::MatrixXd kdTree::k_nearest_neighbors(const Eigen::VectorXd& search_state, int k) const
{
    if (root) {
        if (search_state.size() != state_size) {
            throw BadStateSizeException{};
        }

        return kdTree_k_nearest_neighbors(root, search_state, k);
    }
    else {
        throw EmptyTreeException{};
    }
}

int kdTree::count_states() const
{
    if (root) {
        int depth = 0;
        std::vector<kdNode_ptr> this_level = kdTree_at_depth(root, depth);

        int n_states = 0;
        while (this_level.size()) {
            n_states += this_level.size();
            
            depth++;
            this_level = kdTree_at_depth(root, depth);
        }

        return n_states;
    }
    else {
        return 0;
    }
}