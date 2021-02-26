#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include "vector.hpp"

struct kdNode {
    std::vector<double> state;

    std::shared_ptr<kdNode> left;
    std::shared_ptr<kdNode> right;
};

class kdTree {
public:
    kdTree() {};
    kdTree(const std::vector<std::vector<double>>& states);
    kdTree(std::vector<const std::vector<double>*> state_ptrs);

    void append_state(const std::vector<double>& state);

    std::vector<double> nearest_neighbor(const std::vector<double>& search_state) const;
    std::vector<std::vector<double>> at_depth(int depth) const;

    int count_states() const;

private:
    std::shared_ptr<kdNode> root;
};