#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include "kd_tree.hpp"
#include <iostream>

namespace py = pybind11;

// template<typename V>
struct WrapkdTree {
public:
    WrapkdTree(std::vector<std::vector<double>> states) 
    {
        tree = kdTree(states);
    }

    std::vector<double> nearest_neighbor(std::vector<double> state) const
    {
        std::vector<double> neighbor = tree.nearest_neighbor(state);

        return neighbor;
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
        .def(py::init<std::vector<std::vector<double>>>())
        .def("nearest_neighbor", &WrapkdTree::nearest_neighbor)
        .def("count_states", &WrapkdTree::count_states)
        .def("at_depth", &WrapkdTree::at_depth);
}