#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <exception>
#include "kd_tree.hpp"

namespace py = pybind11;

struct WrapkdTree {
public:
    WrapkdTree(Eigen::Ref<const Eigen::MatrixXd> states)
    {
        tree = kdTree(states);
    }

    Eigen::VectorXd nearest_neighbor(Eigen::Ref<const Eigen::VectorXd> state) const
    {
        Eigen::VectorXd neighbor = tree.nearest_neighbor(state);

        return neighbor;
    }

    int count_states() const 
    {
        return tree.count_states();
    }

    Eigen::MatrixXd at_depth(int depth) const
    {
        std::vector<Eigen::VectorXd> states = tree.at_depth(depth);

        int n_states = states.size();

        Eigen::MatrixXd stacked_states(n_states, tree.state_size);
        
        for (int i = 0; i < n_states; i++)
            stacked_states.row(i) = states[i];

        return stacked_states;
    }

private:
    kdTree tree;
};

void init_kd_tree(py::module_ &m)
{
    py::class_<WrapkdTree>(m, "KD_Tree")
        .def(py::init<Eigen::Ref<const Eigen::MatrixXd>>())
        .def("nearest_neighbor", &WrapkdTree::nearest_neighbor)
        .def("count_states", &WrapkdTree::count_states)
        .def("at_depth", &WrapkdTree::at_depth);
    
    py::register_exception<BadStateSizeException>(m, "BadStateSizeException");
}