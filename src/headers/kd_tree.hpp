#pragma once

#include <memory>
#include <vector>
#include <Eigen/Core>

/* Custom Exception for disagreeing dimensions */

struct VaryingStateSizeException : public std::exception
{
    const char* what() const throw()
    {
        return "Cannot build a tree with varying state sizes.";
    }
};

struct BadStateSizeException : public std::exception
{
    const char* what() const throw()
    {
        return "Given state does not have the same size as tree's state.";
    }
};

/* Tree Node Class */

struct kdNode {
    Eigen::VectorXd state;

    std::shared_ptr<kdNode> left;
    std::shared_ptr<kdNode> right;
};

/* Tree Class */

class kdTree {
public:
    kdTree() {};
    kdTree(const std::vector<Eigen::VectorXd>& states);
    kdTree(const Eigen::MatrixXd& states);
    kdTree(Eigen::Ref<const Eigen::MatrixXd> states);

    int state_size;

    void append_state(const Eigen::VectorXd& state);

    Eigen::VectorXd nearest_neighbor(const Eigen::VectorXd& search_state) const;

    std::vector<Eigen::VectorXd> at_depth(int depth) const;

    int count_states() const;

private:
    std::shared_ptr<kdNode> root;
};