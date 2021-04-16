#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include "kd_tree.hpp"
#include "exceptions.hpp"


namespace py = pybind11;

void init_kd_tree(py::module_ &m)
{
    py::class_<kdTree>(m, "KD_Tree")
        .def(py::init<Eigen::Ref<const Eigen::MatrixXd>>())
        .def("append_state", &kdTree::append_state)
        .def("nearest_neighbor", &kdTree::nearest_neighbor)
        .def("k_nearest_neighbors", &kdTree::k_nearest_neighbors)
        .def("count_states", &kdTree::count_states)
        .def("at_depth", &kdTree::at_depth);
    
    py::register_exception<BadStateSizeException>(m, "BadStateSizeException");
    py::register_exception<EmptyTreeException>(m, "EmptyTreeException");
}