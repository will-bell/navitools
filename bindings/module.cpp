#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>


namespace py = pybind11;

void init_geometry(py::module_ &);
void init_kd_tree(py::module_ &);
void init_roadmap(py::module_ &);
void init_search_space(py::module_ &);

PYBIND11_MODULE(_navitools, m)
{   
    init_geometry(m);
    init_kd_tree(m);
    init_roadmap(m);
    init_search_space(m);
}