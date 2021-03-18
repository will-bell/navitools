#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include "kd_tree.hpp"
#include "exceptions.hpp"

namespace py = pybind11;

struct WrapkdTree {
public:
    WrapkdTree(Eigen::Ref<const Eigen::MatrixXd> states)
    {
        tree = kdTree{states};
    }

    void append_state(Eigen::Ref<const Eigen::VectorXd> state)
    {
        tree.append_state(state);
    }

    Eigen::VectorXd nearest_neighbor(Eigen::Ref<const Eigen::VectorXd> state) const
    {
        return tree.nearest_neighbor(state);
    }

    Eigen::MatrixXd k_nearest_neighbors(Eigen::Ref<const Eigen::MatrixXd> state, int k) const
    {
        return tree.k_nearest_neighbors(state, k);
    }

    int count_states() const 
    {
        return tree.count_states();
    }

    Eigen::MatrixXd at_depth(int depth) const
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
        .def("append_state", &WrapkdTree::append_state)
        .def("nearest_neighbor", &WrapkdTree::nearest_neighbor)
        .def("k_nearest_neighbors", &WrapkdTree::k_nearest_neighbors)
        .def("count_states", &WrapkdTree::count_states)
        .def("at_depth", &WrapkdTree::at_depth);
    
    py::register_exception<BadStateSizeException>(m, "BadStateSizeException");
    py::register_exception<EmptyTreeException>(m, "EmptyTreeException");
}