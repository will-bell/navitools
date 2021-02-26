#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void init_vector(py::module_ &);
void init_geometry(py::module_ &);
void init_kd_tree(py::module_ &);

PYBIND11_MODULE(_navipy, m)
{   
    init_vector(m);
    // init_graph(m);
    init_kd_tree(m);
}