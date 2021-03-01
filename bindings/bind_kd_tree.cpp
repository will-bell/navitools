#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include "kd_tree.hpp"

namespace py = pybind11;

// template<typename V>
struct WrapkdTree {
public:
    WrapkdTree(Eigen::Ref<const Eigen::MatrixXd> states)
    {
        std::vector<std::vector<double>> copy_states;
        for (int i = 0; i < states.rows(); i++) {
            std::vector<double> copy_row;
            for (int j = 0; j < states.cols(); j++) {
                copy_row.push_back(states(i, j));
            }
            copy_states.push_back(copy_row);
        }

        tree = kdTree(copy_states);
    }

    Eigen::VectorXd nearest_neighbor(Eigen::Ref<const Eigen::VectorXd> state) const
    {
        int dim = state.rows();
        if (dim != tree.dim) {
            // Raise exception: dimensions must match
        }

        std::vector<double> copy_state;
        for (int i = 0; i < state.rows(); i++) {
            copy_state.push_back(state(i));
        }

        std::vector<double> neighbor = tree.nearest_neighbor(copy_state);

        Eigen::VectorXd array_neighbor(dim);

        for (int i = 0; i < neighbor.size(); i++) {
            array_neighbor(i) = neighbor[i];
        }
        
        return array_neighbor;
    }

    int count_states() const 
    {
        return tree.count_states();
    }

    std::vector<std::vector<double>> at_depth(int depth) const
    {
        return tree.at_depth(depth);
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
}